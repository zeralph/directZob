#include "Engine.h"
#include <algorithm>
#include "TextureTest.h"
#include <thread> 
#ifdef LINUX
#include <unistd.h>
#else
#include <windows.h>
#endif //LINUX
#include <assert.h> 
#include "DirectZob.h"


using namespace Core;
Engine::Engine(int width, int height, Events* events)
{
	m_nbRasterizers = std::thread::hardware_concurrency() - 1;
	if (m_nbRasterizers % 2 == 1)
	{
		m_nbRasterizers--;
	}
	if (m_nbRasterizers < 2)
	{
		m_nbRasterizers = 1;
	}
	m_events = events;
	m_currentFrame = 0;
	m_zNear = Z_NEAR;
	m_zFar = Z_FAR;
	m_buffer = (uint*)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float*)malloc(sizeof(float) * width * height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_fps = 0.0;
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar= m_zFar;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.size = width * height;
	m_tick = clock();
	m_nbPixels = 0;
	m_renderTimeMS = 0;
	m_geometryTimeMS = 0;
	
	m_rasterTriangleQueues = new std::vector<const Triangle*>[m_nbRasterizers];
	m_rasterLineQueues = new std::vector<Line2D>[m_nbRasterizers];
	m_rasterizers = new std::vector<Rasterizer*>[m_nbRasterizers];

	int h = height / m_nbRasterizers;
	int h0 = 0;
	m_rasterizers->clear();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer* r = new Rasterizer(width, h0, h0+h, &m_bufferData);
		m_rasterizers->push_back(r);
		m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->Init();
	}
	m_events->AddEvent(0, "Engine initialized with " + std::to_string(m_nbRasterizers) + " rasterizer(s)\n");
}

Engine::~Engine()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
	
		m_rasterizers->at(i)->End();
	}
	m_events = NULL;
}

void Engine::Resize(int width, int height)
{
	WaitForRasterizersEnd();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->End();
	}
	delete m_buffer;
	delete m_zBuffer;
	m_buffer = (uint*)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float*)malloc(sizeof(float) * width * height);

	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar = m_zFar;
	m_bufferData.size = width * height;

	for (int i = 0; i < m_nbRasterizers; i++)
	{
		//m_rasterizers->at(i)->End();
		delete m_rasterizers->at(i);
		m_rasterizers->at(i) = NULL;
	}

	int h = height / m_nbRasterizers;
	int h0 = 0;
	m_rasterizers->clear();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer* r = new Rasterizer(width, h0, h0 + h, &m_bufferData);
		m_rasterizers->push_back(r);
		m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->Init();
	}

	m_tick = clock();
}

void Engine::ClearBuffer(const Color* color)
{
	uint v = color->GetRawValue();
	v = 0x3F95FF;
	for (int i = 0; i < m_bufferData.width * m_bufferData.height; i++)
	{
		m_buffer[i] = v;
		m_zBuffer[i] = 0;
	}
	//memset(m_zBuffer, 0, sizeof(float) * m_width * m_height);
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
	}
}

int Engine::DrawScene()
{
	if (!m_started)
	{
		return 0;
	}
	clock_t t;
	t = clock();
	m_sceneTriangles = 0;
	m_drawnTriangles = 0;
	m_geometryTimeMS = (float)(clock() - t) / CLOCKS_PER_SEC * 1000;

	t = clock();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->Start(&m_rasterTriangleQueues[i], &m_rasterLineQueues[i], m_wireFrame);

	}
	WaitForRasterizersEnd();

	m_renderTimeMS = (float)(clock() - t) / CLOCKS_PER_SEC * 1000;

	if (m_showZBuffer)
	{
		uint c;
		for (int i = 0; i < m_bufferData.size; i++)
		{
			c = (uint)(m_zBuffer[i] * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[i] = c;
		}
	}
	int r = 0;
	//r = mfb_update(window, m_buffer);
	m_currentFrame++;
	m_nbPixels = 0;
	m_fps = (float)CLOCKS_PER_SEC / (float)(clock() - m_tick);
	m_tick = t;
	return r;
}

void Engine::WaitForRasterizersEnd() const
{
	bool bWait = true;
	while (bWait)
	{
		bWait = false;
		for (int i = 0; i < m_nbRasterizers; i++)
		{
			bWait |= m_rasterizers->at(i)->m_started;
		}
		//Sleep(1);
	}
}

void Engine::DrawGrid(const Camera* camera)
{
	Vector3 a;
	Vector3 b;
	for (float i = -10; i < 10; i += 1.0f)
	{
		a.x = i;
		b.x = i;
		a.y = 0.0f;
		b.y = 0.0f;
		a.z = -10;
		b.z = 10;
		QueueLine(camera, &a, &b, 0xFFFFFF);
	}
	for (float i = -10; i <= 10; i += 1.0f)
	{
		a.z = b.z = i;
		a.y = b.y = 0.0f;
		a.x = -10;
		b.x = 10;
		QueueLine(camera, &a, &b, 0xFFFFFF);
	}

	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3X, 0xFF0000);
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Y, 0x00FF00);
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0x0000FF);
	a.x = b.x = 1;
	a.y = b.y = 0.0f;
	a.z = -10;
	b.z = 10;
	//QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0xFFFFFF, GetBufferData());
}

void Engine::QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c)
{
	Vector3 a = Vector3(v1);
	Vector3 b = Vector3(v2);

	camera->GetViewMatrix()->Mul(&a);
	camera->GetProjectionMatrix()->Mul(&a);
	camera->GetViewMatrix()->Mul(&b);
	camera->GetProjectionMatrix()->Mul(&b);

	if (ClipSegment(&a, &b))
	{
		if (a.w != 1)
		{
			a.x /= a.w;
			a.y /= a.w;
			a.z /= a.w;
			a.w /= a.w;
		}
		if (b.w != 1)
		{
			b.x /= b.w;
			b.y /= b.w;
			b.z /= b.w;
			b.w /= b.w;
		}
		a.x = (a.x + 1) * m_bufferData.width / 2.0f;
		a.y = (a.y + 1) * m_bufferData.height / 2.0f;
		b.x = (b.x + 1) * m_bufferData.width / 2.0f;
		b.y = (b.y + 1) * m_bufferData.height / 2.0f;
		Line2D l;
		l.xa = a.x;
		l.xb = b.x;
		l.ya = a.y;
		l.yb = b.y;
		l.c = c;
		int min = std::min<int>(a.y, b.y);
		int max = std::max<int>(a.y, b.y);
		min = clamp2(min, 0, (int)m_bufferData.height - 1);
		max = clamp2(max, 0, (int)m_bufferData.height - 1);
		min /= m_bufferData.height / m_nbRasterizers;
		max /= m_bufferData.height / m_nbRasterizers;
		if (min == max)
		{
			int y = 0;
			y++;
		}
		for (int i = min; i <= max; i++)
		{
			m_rasterLineQueues[i].push_back(l);
		}
	}
}

void Engine::CopyBuffer(uint* source, uint * dest)
{
	int v, r, g, b;
	for (int i = 0; i < m_bufferData.size; i++)
	{
		v = source[i];
		r = (int)(float)((v & 0xFF0000) >> 16);
		g = (int)(float)((v & 0x00FF00) >> 8);
		b = (int)(float)((v & 0x0000FF));
		dest[i] = MFB_RGB(r, g, b);
	}
}

bool Engine::ClipSegment(Vector3* a, Vector3* b)
{
	if (a->w < m_zNear && b->w < m_zNear)
	{
		return false;
	}
	else if (a->w < m_zNear || b->w < m_zNear)
	{
		if (a->w < m_zNear)
		{
			float n = (a->w - m_zNear) / (a->w - b->w);
			a->x = (n * a->x) + ((1 - n) * b->x);
			a->y = (n * a->y) + ((1 - n) * b->y);
			a->z = (n * a->z) + ((1 - n) * b->z);
			a->w = m_zNear;
		}
		else
		{
			float n = (b->w - m_zNear) / (b->w - a->w);
			b->x = (n * b->x) + ((1 - n) * a->x);
			b->y = (n * b->y) + ((1 - n) * a->y);
			b->z = (n * b->z) + ((1 - n) * a->z);
			b->w = m_zNear;
		}
	}
	return true;
}

void Engine::QueueTriangle(const Triangle* t)
{
	int min = std::min<int>(t->va->y, std::min<int>(t->vb->y, t->vc->y));
	int max = std::max<int>(t->va->y, std::max<int>(t->vb->y, t->vc->y));
	min = clamp2(min, 0, (int)m_bufferData.height - 1);
	max = clamp2(max, 0, (int)m_bufferData.height - 1);
	min /= m_bufferData.height / m_nbRasterizers;
	max /= m_bufferData.height / m_nbRasterizers;

	assert(min >= 0);
	assert(min < m_nbRasterizers);
	assert(max >= 0);
	assert(max < m_nbRasterizers);
	assert(min <= max);
	for (int i = min; i <= max; i++)
	{
		m_rasterTriangleQueues[i].push_back(t);
	}
}

void Engine::DrawHorizontalLine(const float x1, const float x2, const float y, const uint color)
{
	int k;
	float alpha = 1;// Color::GetAlpha(color);
	if (alpha != 0 && y >= 0 && y < m_bufferData.height)
	{
		if (x1 != x2)
		{
			float a = fmin(x1, x2);
			float b = fmax(x1, x2);
			b++;
			a = (int)(a < 0.0f ? 0.0f : a);
			b = (int)(b > m_bufferData.width ? m_bufferData.width : b);
			for (int i = a; i < b; i++)
			{
				k = y * m_bufferData.width + i;
				uint g = m_bufferData.buffer[k];
				m_bufferData.buffer[k] = color;
			}
		}
	}
}
