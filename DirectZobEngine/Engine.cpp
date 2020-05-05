#include "Engine.h"
#include <algorithm>
#include <thread>
#ifdef LINUX
#include <unistd.h>
#elif MACOS
#include <unistd.h>
#elif WINDOWS
#include <windows.h>
#endif //LINUX
#include <assert.h>
#include "DirectZob.h"
#include "Rasterizer.h"
#include "Camera.h"

#define MAX_TRIANGLES_PER_IMAGE 400000

using namespace Core;
Engine::Engine(int width, int height, Events* events)
{
	m_nbRasterizers = std::thread::hardware_concurrency();
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
	m_buffer = (uint *)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float *)malloc(sizeof(float) * width * height);
	//	m_oBuffer = (uint*)malloc(sizeof(uint) * width * height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_fps = 0.0;
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar = m_zFar;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	//	m_bufferData.oBuffer = m_oBuffer;
	m_bufferData.size = width * height;
	m_tick = clock();
	m_nbPixels = 0;
	m_renderTimeMS = 0;
	m_geometryTimeMS = 0;
	m_cullMode = CullMode::CullClockwiseFace;

	m_rasterTriangleQueues = (Triangle **)malloc(sizeof(Triangle) * m_nbRasterizers);
	m_rasterNbTriangleQueues = (long *)malloc(sizeof(long) * m_nbRasterizers);
	m_rasterLineQueues = new std::vector<Line3D>[m_nbRasterizers];
	//m_rasterizers = new std::vector<Rasterizer*>[m_nbRasterizers];
	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	int h = height / m_nbRasterizers;
	int h0 = 0;
	
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer *r = new Rasterizer(width, h0, h0 + h, &m_bufferData);
		m_rasterizers[i] = r;

		Triangle *t = (Triangle *)malloc(sizeof(Triangle) * m_triangleQueueSize);
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
		m_rasterizers[i]->Init();
	}
	std::string n = "Engine initialized with " + std::to_string(m_nbRasterizers) + " rasterizer(s) for " + std::to_string(m_triangleQueueSize) + " triangles per image";
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

		m_rasterizers[i]->End();
	}
	m_events = NULL;
}

void Engine::Stop()
{
	WaitForRasterizersEnd();
	ClearRenderQueues();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Clear();
	}
	m_started = false;
}

void Engine::Resize(int width, int height)
{
	bool bStarted = m_started;
	Stop();
	SLEEP(100);
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->End();
	}
	delete m_buffer;
	delete m_zBuffer;
	//	delete m_oBuffer;
	m_buffer = (uint *)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float *)malloc(sizeof(float) * width * height);
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
		delete m_rasterizers[i];
		m_rasterizers[i] = NULL;
	}

	int h = height / m_nbRasterizers;
	int h0 = 0;
	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer *r = new Rasterizer(width, h0, h0 + h, &m_bufferData);
		m_rasterizers[i] = r;
		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
		h0 += h;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Init();
	}
	if (bStarted)
	{
		Start();
	}
	m_tick = clock();
}

void Engine::ClearBuffer(const Color *color)
{
	m_tick = clock();
	uint v = color->GetRawValue();
	//Color cc = Color(63, 149, 255, 255);
	//v = cc.GetRawValue();
	if (!m_scaneLine)
	{
		//memset(m_zBuffer, 0, sizeof(float) * m_bufferData.width * m_bufferData.height);
		for (int i = 0; i < m_bufferData.width * m_bufferData.height; i++)
		{
			m_zBuffer[i] = -1.0f;
			m_buffer[i] = v;
		}
	}
	else
	{	
		for (int y = (m_currentFrame) % 2; y < m_bufferData.height; y += 2)
		{
			int s = m_bufferData.width * y;
			for (int i = 0; i < m_bufferData.width; i++)
			{
				m_zBuffer[i+s] = -1.0f;
				m_buffer[i+s] = v;
			}
		}
	}
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
		m_rasterizers[i]->Start(m_rasterTriangleQueues[i], m_rasterNbTriangleQueues[i], &m_rasterLineQueues[i], m_wireFrame, m_scaneLine, m_currentFrame % 2, m_lightingPrecision);
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
			m_buffer[i] = 0; // c;
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
		//m_rasterTriangleQueues[i]->clear();
		m_rasterLineQueues[i].clear();
		m_rasterNbTriangleQueues[i] = 0;
		m_rasterizers[i]->Clear();
	}
}

void Engine::WaitForRasterizersEnd()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		if (m_rasterizers[i])
		{
			m_rasterizers[i]->WaitForEnd();
		}
	}
	m_renderTimeMS = (float)(clock() - m_drawTick) / CLOCKS_PER_SEC * 1000;
}

void Engine::DrawGrid(const Camera *camera)
{
	int gridSize = 50;
	Vector3 a;
	Vector3 b;
	bool bold;
	for (int i = -gridSize; i <= gridSize; i += 1.0f)
	{
		a.x = i;
		b.x = i;
		a.y = 0.0f;
		b.y = 0.0f;
		a.z = -gridSize;
		b.z = gridSize;
		bold = i % 5 == 0;
		if (bold)
		{
			QueueLine(camera, &a, &b, 0xFFFFFF, bold);
		}
		else
		{
			QueueLine(camera, &a, &b, 0xCCCCCC, bold);
		}
	}
	for (int i = -gridSize; i <= gridSize; i += 1.0f)
	{
		a.z = b.z = i;
		a.y = b.y = 0.0f;
		a.x = -gridSize;
		b.x = gridSize;
		bold = i % 5 == 0;
		if (bold)
		{
			QueueLine(camera, &a, &b, 0xFFFFFF, bold);
		}
		else
		{
			QueueLine(camera, &a, &b, 0xCCCCCC, bold);
		}
	}

	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3X, 0xFF0000, true);
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Y, 0x00FF00, true);
	QueueLine(camera, &Vector3::Vector3Zero, &Vector3::Vector3Z, 0x0000FF, true);
}

void Engine::ClipSegmentToPlane(Vector3 &s0, Vector3 &s1, Vector3 &pp, Vector3 &pn)
{
	Vector3 u = s1 - s0;
	Vector3 w = s0 - pp;
	float D = Vector3::Dot(&pn, &u);
	float N = -Vector3::Dot(&pn, &w);
	if (fabs(D) < 0.001f)
	{
		return;
	}
	float sI = N / D;
	if (sI < 0 || sI > 1)
	{
		return;
	}
	u.Mul(sI);
	s1 = s0 + u;
}

void Engine::QueueEllipse(const Camera* camera, const Vector3* center, const Vector3* up, const float r1, const float r2, const uint c, bool bold)
{
	int segs = 10;
	Matrix4x4 m;
	Vector3 y = Vector3(0, 1, 0);
	Vector3 x = Vector3(1, 0, 0);
	if (y != up)
	{
		x = Vector3::Cross(up, &y);
	}
	Vector3 z = Vector3::Cross(&x, up);
	y = up;
	y.Normalize();
	x.Normalize();
	z.Normalize();
	m.SetData(0,0, x.x);
	m.SetData(0,1, y.x);
	m.SetData(0,2, z.x);
	m.SetData(1,0, x.y);
	m.SetData(1,1, y.y);
	m.SetData(1,2, z.y);
	m.SetData(2,0, x.z);
	m.SetData(2,1, y.z);
	m.SetData(2,2, z.z);
	Vector3 a = Vector3(r1, 0, 0);
	Vector3 b = a;
	float rot = 360.0f / (float)segs;
	m.SetRotation(0, rot, 0);
	m.Mul(&a);
	a = a + center;
	for(int i=0; i<segs; i++)
	{
		b = a;
		a = Vector3(r1, 0, 0);
		m.SetRotation(0, rot, 0);
		m.Mul(&a);
		a = a + center;
		QueueLine(camera, &a, &b, c, bold);
	}

}

void Engine::QueueLine(const Camera *camera, const Vector3 *v1, const Vector3 *v2, const uint c, bool bold)
{
	if (m_started)
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

		if (a.z < m_bufferData.zNear && b.z < m_bufferData.zNear)
		{
			return;
		}
		else if (a.z < m_bufferData.zNear || b.z < m_bufferData.zNear)
		{
			Vector3 pp = Vector3(0, 0, m_bufferData.zNear);
			Vector3 pn = Vector3(0, 0, 1);
			if (a.z < b.z)
			{
				ClipSegmentToPlane(b, a, pp, pn);
			}
			else
			{
				ClipSegmentToPlane(a, b, pp, pn);
			}
		}
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
			l.bold = bold;
			int min = std::min<int>(a.y, b.y);
			int max = std::max<int>(a.y, b.y);
			min = clamp2(min, 0, (int)m_bufferData.height - 1);
			max = clamp2(max, 0, (int)m_bufferData.height - 1);
			min /= m_bufferData.height / m_nbRasterizers;
			max /= m_bufferData.height / m_nbRasterizers;
//			min = 0;
//			max = m_nbRasterizers-1;
			for (int i = min; i <= max; i++)
			{
				m_rasterLineQueues[i].push_back(l);
			}
		}
	}
}

void Engine::CopyBuffer(uint *source, uint *dest)
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

bool Engine::ClipSegment(Vector3 *a, Vector3 *b)
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

void Engine::QueueTriangle(const Triangle *t)
{
	if (m_started)
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
				memcpy(&m_rasterTriangleQueues[i][j], t, sizeof(Triangle));
				//m_rasterTriangleQueues[i][j].Copy(t);
				m_rasterNbTriangleQueues[i]++;
				m_drawnTriangles++;
			}
			m_sceneTriangles++;
		}
	}
}

void Engine::DrawHorizontalLine(const float x1, const float x2, const float y, const uint color)
{
	int k;
	float alpha = 1; // Color::GetAlpha(color);
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
