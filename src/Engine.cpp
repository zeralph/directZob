#include "Engine.h"
#include <algorithm>
#include "TextureTest.h"
#include <thread> 
#include <windows.h>
#include <assert.h> 

Engine::Engine(int width, int height)
{
	m_currentFrame = 0;
	m_width = width;
	m_height = height;
	m_zNear = Z_NEAR;
	m_zFar = Z_FAR;
	m_buffer = (uint*)malloc(sizeof(uint) * m_width * m_height);
	m_zBuffer = (float*)malloc(sizeof(float) * m_width * m_height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_fps = 0.0;
	m_bufferData.height = m_height;
	m_bufferData.width = m_width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar= m_zFar;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.size = m_width * m_height;
	m_tick = clock();
	m_nbPixels = 0;
	m_renderTimeMS = 0;
	m_geometryTimeMS = 0;


	int h = m_height / NB_RASTERIZERS;
	int h0 = 0;
	for (int i = 0; i < NB_RASTERIZERS; i++)
	{
		m_rasterizers[i] = new Rasterizer(m_width, h0, h0+h, m_zNear, m_zFar);
		m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	
	//m_rasterizer2 = new Rasterizer(m_width, m_height / 2, m_height, m_zNear, m_zFar);
	//std::thread r2(&m_rasterizer2->Render);
	//m_camera = new Camera();
	//m_camera->setProjectionMatrix(90.0f, width, height, m_zNear, m_zFar);
}

Engine::~Engine()
{

}

void Engine::Add(Mesh* mesh)
{
	if(mesh)
		m_meshes.push_back(mesh);
}

void Engine::Resize(int width, int height)
{
	m_width = width;
	m_height = height;

	delete m_buffer;
	delete m_zBuffer;
	m_buffer = (uint*)malloc(sizeof(uint) * m_width * m_height);
	m_zBuffer = (float*)malloc(sizeof(float) * m_width * m_height);

	m_bufferData.height = m_height;
	m_bufferData.width = m_width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar = m_zFar;
	m_bufferData.size = m_width * m_height;
	m_tick = clock();
	//	m_camera->setProjectionMatrix(90.0f, width, height, m_zNear, m_zFar);
}

void Engine::ClearBuffer(const Color* color)
{
	uint v = color->GetRawValue();
	memset(m_buffer, v, sizeof(uint) * m_width * m_height);
	memset(m_zBuffer, 0, sizeof(float) * m_width * m_height);
	for (int i = 0; i < NB_RASTERIZERS; i++)
	{
		m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
	}
}

int Engine::Update(struct Window *window, const Camera* camera)
{
	clock_t t;
	t = clock();
	m_sceneTriangles = 0;
	m_drawnTriangles = 0;
	for (int i = 0; i < m_meshes.size(); i++)
	{
		Mesh* m = m_meshes.at(i);
		m->Update(camera, &m_bufferData);
		const std::vector<Triangle>* tList = m->GetTrianglesList();
		for (int j = 0; j < tList->size(); j++)
		{
			const Triangle* t = &tList->at(j);
			if (t->draw)
			{
				this->QueueTriangle(t);
				m_drawnTriangles++;
			}
			m_sceneTriangles++;
		}
	}
	m_geometryTimeMS = (float)(clock() - t) / CLOCKS_PER_SEC * 1000;

	t = clock();
	for (int i = 0; i < NB_RASTERIZERS; i++)
	{
		m_rasterizers[i]->Start(&m_rasterTriangleQueues[i], &m_rasterLineQueues[i], &m_bufferData);

	}
	for (int i = 0; i < NB_RASTERIZERS; i++)
	{
		m_rasterThreads[i] = std::thread(&Rasterizer::Run, m_rasterizers[i]);
	}
	for (int i = 0; i < NB_RASTERIZERS; i++)
	{
		m_rasterThreads[i].join();
	}
	m_renderTimeMS = (float)(clock() - t) / CLOCKS_PER_SEC * 1000;

	if (m_showZBuffer)
	{
		uint c;
		for (int i = 0; i < m_width*m_height; i++)
		{
			c = (uint)(m_zBuffer[i] * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[i] = c;
		}
	}
	int r = 0;
	r = mfb_update(window, m_buffer);
	m_currentFrame++;
	m_nbPixels = 0;
	m_fps = (float)CLOCKS_PER_SEC / (float)(clock() - m_tick);
	m_tick = t;
	return r;
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
		QueueLine(camera, &a, &b, 0xFFFFFF, GetBufferData());
	}
	for (float i = -10; i <= 10; i += 1.0f)
	{
		a.z = b.z = i;
		a.y = b.y = 0.0f;
		a.x = -10;
		b.x = 10;
		QueueLine(camera, &a, &b, 0xFFFFFF, GetBufferData());
	}

	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3X, 0xFF0000, GetBufferData());
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Y, 0x00FF00, GetBufferData());
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0x0000FF, GetBufferData());
	a.x = b.x = 1;
	a.y = b.y = 0.0f;
	a.z = -10;
	b.z = 10;
	//QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0xFFFFFF, GetBufferData());
}

void Engine::QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c, BufferData* bufferData)
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
		a.x = (a.x + 1) * m_width / 2.0f;
		a.y = (a.y + 1) * m_height / 2.0f;
		b.x = (b.x + 1) * m_width / 2.0f;
		b.y = (b.y + 1) * m_height / 2.0f;
		Line2D l;
		l.xa = a.x;
		l.xb = b.x;
		l.ya = a.y;
		l.yb = b.y;
		l.c = c;
		int min = std::min<int>(a.y, b.y);
		int max = std::max<int>(a.y, b.y);
		min = clamp2(min, 0, m_height - 1);
		max = clamp2(max, 0, m_height - 1);
		min /= m_height / NB_RASTERIZERS;
		max /= m_height / NB_RASTERIZERS;
		if (min == max)
		{
			int y = 0;
			y++;
		}
		for (int i = min; i <= max; i++)
		{
			m_rasterLineQueues[i].push_back(l);
		}
		/*
		if (min < m_height / 2 && max < m_height / 2)
		{
			m_rasterLineQueue1.push_back(l);
		}
		else if (min >= m_height / 2 && max >= m_height / 2)
		{
			m_rasterLineQueue2.push_back(l);
		}
		else
		{
			m_rasterLineQueue1.push_back(l);
			m_rasterLineQueue2.push_back(l);
		}
		*/
	}
}

void Engine::CopyBuffer(uint* source, uint * dest)
{
	int v, r, g, b;
	for (int i = 0; i < m_width*m_height; i++)
	{
		v = source[i];
		r = (int)(float)((v & 0xFF0000) >> 16);
		g = (int)(float)((v & 0x00FF00) >> 8);
		b = (int)(float)((v & 0x0000FF));
		dest[i] = MFB_RGB(r, g, b);
	}
}

void Engine::GetPixelColor(Color* color, int x, int y)
{
	int i = y * m_width + x;
	color->Set(m_buffer[i]);
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
	min = clamp2(min, 0, m_height - 1);
	max = clamp2(max, 0, m_height - 1);
	min /= m_height / NB_RASTERIZERS;
	max /= m_height / NB_RASTERIZERS;

	assert(min >= 0);
	assert(min < NB_RASTERIZERS);
	assert(max >= 0);
	assert(max < NB_RASTERIZERS);
	assert(min <= max);
	for (int i = min; i <= max; i++)
	{
		m_rasterTriangleQueues[i].push_back(t);
	}

	/*
	if (min < m_height / 2 && max < m_height / 2)
	{
		m_rasterTriangleQueue1.push_back(t);
	}
	else if (min >= m_height / 2 && max >= m_height / 2)
	{
		m_rasterTriangleQueue2.push_back(t);
	}
	else
	{
		m_rasterTriangleQueue1.push_back(t);
		m_rasterTriangleQueue2.push_back(t);
	}
	*/
}

void Engine::DrawHorizontalLine(const float x1, const float x2, const float y, const uint color, BufferData* bufferData)
{
	int k;
	float alpha = 1;// Color::GetAlpha(color);
	uint* buffer = bufferData->buffer;
	if (alpha != 0 && y >= 0 && y < m_height)
	{
		if (x1 != x2)
		{
			float a = fmin(x1, x2);
			float b = fmax(x1, x2);
			b++;
			a = (int)(a < 0.0f ? 0.0f : a);
			b = (int)(b > m_width ? m_width : b);
			for (int i = a; i < b; i++)
			{
				k = y * m_width + i;
				uint g = buffer[k];
				buffer[k] = color;
			}
		}
	}
}
