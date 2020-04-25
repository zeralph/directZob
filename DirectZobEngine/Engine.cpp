#include "Engine.h"
#include <algorithm>
#include <thread> 
#ifdef LINUX
#include <unistd.h>
#else
#include <windows.h>
#endif //LINUX
#include <assert.h> 
#include "DirectZob.h"

# define MAX_TRIANGLES_PER_IMAGE 200000

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
	m_triangleQueueSize = MAX_TRIANGLES_PER_IMAGE / m_nbRasterizers;
	m_renderOutput = RenderOutput_render;
	m_events = events;
	m_currentFrame = 0;
	m_zNear = Z_NEAR;
	m_zFar = Z_FAR;
	m_buffer = (uint*)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float*)malloc(sizeof(float) * width * height);
//	m_oBuffer = (uint*)malloc(sizeof(uint) * width * height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_fps = 0.0;
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar= m_zFar;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
//	m_bufferData.oBuffer = m_oBuffer;
	m_bufferData.size = width * height;
	m_tick = clock();
	m_nbPixels = 0;
	m_renderTimeMS = 0;
	m_geometryTimeMS = 0;
	m_cullMode = CullMode::CullClockwiseFace;

	m_rasterTriangleQueues = (Triangle**)malloc(sizeof(Triangle) * m_nbRasterizers);
	m_rasterNbTriangleQueues = (uint*)malloc(sizeof(uint) * m_nbRasterizers);
	m_rasterLineQueues = new std::vector<Line3D>[m_nbRasterizers];
	m_rasterizers = new std::vector<Rasterizer*>[m_nbRasterizers];

	int h = height / m_nbRasterizers;
	int h0 = 0;
	m_rasterizers->clear();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer* r = new Rasterizer(width, h0, h0+h, &m_bufferData);
		m_rasterizers->push_back(r);

		Triangle* t = (Triangle*)malloc(sizeof(Triangle) * m_triangleQueueSize);
		m_rasterTriangleQueues[i] = t;
		for (int j = 0; j < m_triangleQueueSize; j++)
		{
			m_rasterTriangleQueues[i][j].va = new Vector3();
			m_rasterTriangleQueues[i][j].vb = new Vector3();
			m_rasterTriangleQueues[i][j].vc = new Vector3();
			m_rasterTriangleQueues[i][j].pa = new Vector3();
			m_rasterTriangleQueues[i][j].pb = new Vector3();
			m_rasterTriangleQueues[i][j].pc = new Vector3();
			m_rasterTriangleQueues[i][j].na = new Vector3();
			m_rasterTriangleQueues[i][j].nb = new Vector3();
			m_rasterTriangleQueues[i][j].nc = new Vector3();
			m_rasterTriangleQueues[i][j].n = new Vector3();
			m_rasterTriangleQueues[i][j].ua = new Vector2();
			m_rasterTriangleQueues[i][j].ub = new Vector2();
			m_rasterTriangleQueues[i][j].uc = new Vector2();
		}
		

		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterLineQueues[i].clear();
		m_rasterNbTriangleQueues[i] = 0;
		m_rasterizers->at(i)->Init();
	}
	std::string n = "Engine initialized with " + std::to_string(m_nbRasterizers) + " rasterizer(s) for " + std::to_string(m_triangleQueueSize) +  " triangles per image";
	DirectZob::LogInfo(n.c_str());
}

uint Engine::GetObjectIdAtCoords(uint x, uint y)
{
	size_t l = m_bufferData.width * y + x;
	if (l >= 0 && l < m_bufferData.width * m_bufferData.height)
	{
//		return m_bufferData.oBuffer[l];
	}
	return 0;
}

Engine::~Engine()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
	
		m_rasterizers->at(i)->End();
	}
	m_events = NULL;
}

void Engine::Stop()
{
	m_started = false;
	WaitForRasterizersEnd();
	ClearRenderQueues();
	Sleep(100);
}

void Engine::Resize(int width, int height)
{
	bool bStarted = m_started;
	Stop();
	Sleep(100);
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->End();
	}
	delete m_buffer;
	delete m_zBuffer;
//	delete m_oBuffer;
	m_buffer = (uint*)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float*)malloc(sizeof(float) * width * height);
//	m_oBuffer = (uint*)malloc(sizeof(float) * width * height);
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
//	m_bufferData.oBuffer = m_oBuffer;
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
		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->Init();
	}
	if (bStarted)
	{
		Start();
	}
	m_tick = clock();
}

void Engine::ClearBuffer(const Color* color)
{
	m_tick = clock();
	uint v = color->GetRawValue();
	for (int i = 0; i < m_bufferData.width * m_bufferData.height; i++)
	{
		m_buffer[i] = v;
		m_zBuffer[i] = -1.0f;
//		m_oBuffer[i] = 0;
	}
	
	/*for (int i = 0; i < m_nbRasterizers; i++)
	{
		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		m_rasterNbTriangleQueues[i] = 0;
	}*/
	m_sceneTriangles = 0;
	m_drawnTriangles = 0;
}

int Engine::StartDrawingScene()
{
	if (!m_started)
	{
		return 0;
	}
	m_drawTick = clock();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers->at(i)->Start(m_rasterTriangleQueues[i], m_rasterNbTriangleQueues[i], &m_rasterLineQueues[i], m_wireFrame);

	}
	return 0;
}
int Engine::EndDrawingScene()
{
	if (m_renderOutput == RenderOutput_zBuffer)
	{
		uint c;
		for (int i = 0; i < m_bufferData.size; i++)
		{
			c = (uint)(m_zBuffer[i] * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[i] = c;
		}
	}
	else if (m_renderOutput == RenderOutput_oBuffer)
	{
		uint c;
		for (int i = 0; i < m_bufferData.size; i++)
		{
			//c = oBufferColors[(uint)(m_oBuffer[i]) % 8];
			m_buffer[i] = 0;// c;
		}
	}
	int r = 0;
	//r = mfb_update(window, m_buffer);



	m_currentFrame++;
	m_nbPixels = 0;
	m_frameTimeMS = (float)(clock() - m_tick) / CLOCKS_PER_SEC * 1000;
	m_fps = (float)CLOCKS_PER_SEC / (float)(clock() - m_tick);
	return r;
}

void Engine::ClearRenderQueues()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		m_rasterNbTriangleQueues[i] = 0;
	}
}

void Engine::WaitForRasterizersEnd() 
{
	bool bWait = true;
	while (bWait)
	{
		bWait = false;
		for (int i = 0; i < m_nbRasterizers; i++)
		{
			bWait |= m_rasterizers->at(i)->HasStarted();
		}
	}
	m_renderTimeMS = (float)(clock() - m_drawTick) / CLOCKS_PER_SEC * 1000;
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

void Engine::ClipSegmentToPlane(Vector3 &s0, Vector3 &s1, Vector3 &pp, Vector3 &pn)
{
	
//std:swap(s0, s1);

	Vector3 u = s1 - s0;
	Vector3 w = s0 - pp;

	float D = Vector3::Dot(&pn, &u);
	float N = -Vector3::Dot(&pn, &w);

	if (fabs(D) < 0.001f) {           // segment is parallel to plane
		/*if (N == 0)                      // segment lies in plane
			return 2;
		else
			return 0;                    // no intersection
		*/
		return;
	}
	// they are not parallel
	// compute intersect param
	float sI = N / D;
	if (sI < 0 || sI > 1)
	{
		//return 0;                        // no intersection
		return;
	}
	
//	s1 = s0 + s1.Mul(u);                  // compute segment intersect point
	/*
	return
	return 1;
	*/
}

void Engine::QueueLine(const Camera* camera, const Vector3* v1, const Vector3* v2, const uint c)
{
	Vector3 a = Vector3(v1);
	Vector3 b = Vector3(v2);
	float za, zb = 0.0f;
	camera->GetViewMatrix()->Mul(&a);
	za = a.z;
	camera->GetProjectionMatrix()->Mul(&a);
	camera->GetViewMatrix()->Mul(&b);
	zb = b.z;
	camera->GetProjectionMatrix()->Mul(&b);
	
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
	//if (ClipSegment(&a, &b))
	{

		a.x = (a.x + 1) * m_bufferData.width / 2.0f;
		a.y = (a.y + 1) * m_bufferData.height / 2.0f;
		b.x = (b.x + 1) * m_bufferData.width / 2.0f;
		b.y = (b.y + 1) * m_bufferData.height / 2.0f;
		Line3D l;
		l.xa = a.x;
		l.xb = b.x;
		l.ya = a.y;
		l.yb = b.y;
		l.za = za;
		l.zb = zb;
		l.c = c;
		int min = std::min<int>(a.y, b.y);
		int max = std::max<int>(a.y, b.y);
		min = clamp2(min, 0, (int)m_bufferData.height - 1);
		max = clamp2(max, 0, (int)m_bufferData.height - 1);
		min /= m_bufferData.height / m_nbRasterizers;
		max /= m_bufferData.height / m_nbRasterizers;
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
	int min = std::min<int>(t->pa->y, std::min<int>(t->pb->y, t->pc->y));
	int max = std::max<int>(t->pa->y, std::max<int>(t->pb->y, t->pc->y));
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
		uint j = m_rasterNbTriangleQueues[i];
		if (j < m_triangleQueueSize)
		{
			m_rasterTriangleQueues[i][j].Copy(t);
			m_rasterNbTriangleQueues[i]++;
			m_drawnTriangles++;
		}
		else
		{
			int oo = 0;
			oo++;
		}
		m_sceneTriangles++;
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
