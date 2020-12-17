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
#include <mutex>
#include <condition_variable>
#include "DirectZob.h"
#include "Rasterizer.h"
#include "../ZobObjects/Camera.h"

#define MAX_TRIANGLES_PER_IMAGE 400000

static std::mutex m_mutex;
static std::condition_variable m_cond;

using namespace Core;
Engine::Engine(int width, int height, Events* events)
{
	m_started = false;
	m_wireFrame = false;
	m_rasterizerHeight = 0;
	m_showNormals = false;
	m_showGrid = true;
	m_drawGizmos = true;
	m_showBBoxes = true;
	m_lockFrustrum = false;
	m_nbRasterizers = std::thread::hardware_concurrency();
	while (height % m_nbRasterizers != 0 && m_nbRasterizers>1)
	{
		m_nbRasterizers--;
	}
	/*
	if (m_nbRasterizers % 2 == 1)
	{
		m_nbRasterizers--;
	}
	if (m_nbRasterizers < 2)
	{
		m_nbRasterizers = 1;
	}
	m_nbRasterizers = 4;
	*/
	//m_nbRasterizers = 4;
	m_maxTrianglesQueueSize = 200000;// MAX_TRIANGLES_PER_IMAGE / m_nbRasterizers;
	m_maxLineQueueSize = 1000;
	m_renderOutput = eRenderOutput_render;
	m_events = events;
	m_currentFrame = 0;
	m_zNear = 0.11f;
	m_zFar = 1000.0f;
	m_buffer = (uint *)malloc(sizeof(uint) * width * height);
	m_zBuffer = (float *)malloc(sizeof(float) * width * height);
	//	m_oBuffer = (uint*)malloc(sizeof(uint) * width * height);
	m_curBuffer = 0;
	m_showZBuffer = false;
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar = m_zFar;
	m_bufferData.buffer = m_buffer;
	m_bufferData.zBuffer = m_zBuffer;
	//	m_bufferData.oBuffer = m_oBuffer;
	m_bufferData.size = width * height;
	m_nbPixels = 0;

	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	m_rasterizerHeight = ceil((float)height / (float)m_nbRasterizers);
	int h0 = 0;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer* r = new Rasterizer(width, h0, h0 + m_rasterizerHeight, &m_bufferData);
		m_rasterizers[i] = r;
		h0 += m_rasterizerHeight;
	}

	m_LineQueue = (Line3D*)malloc(sizeof(Line3D) * m_maxLineQueueSize);
	m_TrianglesQueue = (Triangle*)malloc(sizeof(Triangle) * m_maxTrianglesQueueSize);
	for (int i = 0; i < m_maxTrianglesQueueSize; i++)
	{
		m_TrianglesQueue[i].va = new ZobVector3();
		m_TrianglesQueue[i].vb = new ZobVector3();
		m_TrianglesQueue[i].vc = new ZobVector3();
		m_TrianglesQueue[i].pa = new ZobVector3();
		m_TrianglesQueue[i].pb = new ZobVector3();
		m_TrianglesQueue[i].pc = new ZobVector3(); 
		m_TrianglesQueue[i].na = new ZobVector3(); 
		m_TrianglesQueue[i].nb = new ZobVector3(); 
		m_TrianglesQueue[i].nc = new ZobVector3(); 
		m_TrianglesQueue[i].ua = new ZobVector2(); 
		m_TrianglesQueue[i].ub = new ZobVector2(); 
		m_TrianglesQueue[i].uc = new ZobVector2(); 
		m_TrianglesQueue[i].n = new ZobVector3();
		m_TrianglesQueue[i].options = (DirectZobType::RenderOptions*)malloc(sizeof(DirectZobType::RenderOptions));
		m_TrianglesQueue[i].options->zBuffered = true;
		m_TrianglesQueue[i].options->bTransparency = false;
		m_TrianglesQueue[i].options->cullMode = eCullMode_ClockwiseFace;
		m_TrianglesQueue[i].draw = false;
		m_TrianglesQueue[i].area = 0;
		m_TrianglesQueue[i].material = NULL;
		m_TrianglesQueue[i].zobObject = NULL;
		m_TrianglesQueue[i].ca = 0;
		m_TrianglesQueue[i].cb = 0;
		m_TrianglesQueue[i].cc = 0;
	}
	m_lineQueueSize = 0;
	m_TriangleQueueSize = 0;
	for (int i = 0; i < m_nbRasterizers; i++)
	{

		m_rasterizers[i]->Init();
	}
	std::string n = "Engine initialized with " + std::to_string(m_nbRasterizers) + " rasterizer(s) for " + std::to_string(m_maxTrianglesQueueSize) + " triangles per image";
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
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		delete m_rasterizers[i];
		delete m_TrianglesQueue;
	}
	delete m_TrianglesQueue;
	delete m_rasterizers;
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

	m_rasterizerHeight = ceil( (float)height / (float)m_nbRasterizers);
	int h0 = 0;
	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer *r = new Rasterizer(width, h0, h0 + m_rasterizerHeight, &m_bufferData);
		m_rasterizers[i] = r;
		//m_TrianglesQueue[i].clear();
		//m_rasterLineQueues.clear();
		h0 += m_rasterizerHeight;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Init();
	}
	if (bStarted)
	{
		Start();
	}
}

void Engine::ClearBuffer(const Color *color)
{
	uint v = color->GetRawValue();
	//Color cc = Color(63, 149, 255, 255);
	//v = cc.GetRawValue();
	if (m_renderMode == eRenderMode_fullframe )
	{
		//memset(m_zBuffer, 0, sizeof(float) * m_bufferData.width * m_bufferData.height);
		for (int i = 0; i < m_bufferData.width * m_bufferData.height; i++)
		{
			m_zBuffer[i] = 1.0f;
			m_buffer[i] = v;
		}
	}
	else if (m_renderMode == eRenderMode_scanline)
	{
		uint c;
		for (int y = 0; y < m_bufferData.height; y ++)
		{
			if (y % 2)
			{
				c = v;
			}
			else
			{
				c = 0;
			}
			int s = m_bufferData.width * y;
			for (int i = 0; i < m_bufferData.width; i++)
			{
				m_zBuffer[i + s] = 1.0f;
				m_buffer[i + s] = c;
			}
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
	m_drawnTriangles = 0;
}

int Engine::StartDrawingScene()
{
	if (!m_started)
	{
		return 0;
	}
	const ZobVector3 camForward = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()->GetForward();
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Start(m_wireFrame, m_renderMode, m_currentFrame % 2, m_lightingPrecision, camForward);
	}
	return 0;
}
int Engine::SetDisplayedBuffer()
{
	if (m_renderOutput == eRenderOutput_zBuffer)
	{
		uint c;
		for (int i = 0; i < m_bufferData.size; i++)
		{
			c = (uint)(m_zBuffer[i] * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[i] = c;
		}
	}
	else if (m_renderOutput == eRenderOutput_oBuffer)
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
	return r;
}

void Engine::ClearRenderQueues()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_lineQueueSize = 0;
		m_TriangleQueueSize = 0;
		m_rasterizers[i]->Clear();
	}
}

float Engine::WaitForRasterizersEnd()
{
	float t = 0.0f;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		if (m_rasterizers[i])
		{
			float t2 = m_rasterizers[i]->WaitForEnd();
			t = fmax(t, t2);
		}
	}
	return t;
}

void Engine::DrawGrid(const Camera *camera)
{
	int gridSize = 50;
	ZobVector3 a;
	ZobVector3 b;
	bool bold;
	float cx = 0; // (int)(camera->GetWorldPosition().x);
	float cz = 0; // (int)(camera->GetWorldPosition().z);
	for (int i = -gridSize; i <= gridSize; i += 1.0f)
	{
		a.x = i + cx;
		b.x = i + cx;
		a.y = 0.0f;
		b.y = 0.0f;
		a.z = -gridSize + cz;
		b.z = gridSize + cz;
		bold = i % 5 == 0;
		if (bold)
		{
			QueueLine(camera, &a, &b, 0xFFFFFF, bold, false);
		}
		else
		{
			QueueLine(camera, &a, &b, 0xCCCCCC, bold, false);
		}
	}
	for (int i = -gridSize; i <= gridSize; i += 1.0f)
	{
		a.z = b.z = i +cz;
		a.y = b.y = 0.0f;
		a.x = -gridSize + cx;
		b.x = gridSize + cx;
		bold = i % 5 == 0;
		if (bold)
		{
			QueueLine(camera, &a, &b, 0xFFFFFF, bold, false);
		}
		else
		{
			QueueLine(camera, &a, &b, 0xCCCCCC, bold, false);
		}
	}

	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3X, 0xFF0000, true, false);
	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Y, 0x00FF00, true, false);
	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Z, 0x0000FF, true, false);
}

bool Engine::LinePlaneIntersection(const ZobVector3* planeOrigin, const ZobVector3* planeNormal, const ZobVector3* lineOrigin, const ZobVector3* lineDirection, ZobVector3* ret)
{
	float d = -(planeNormal->x * planeOrigin->x + planeNormal->y * planeOrigin->y + planeNormal->z * planeOrigin->z);
	float t = -(planeNormal->x * lineOrigin->x + planeNormal->y * lineOrigin->y + planeNormal->z * lineOrigin->z + d) / (planeNormal->x * lineDirection->x + planeNormal->y * lineDirection->y + planeNormal->z * lineDirection->z);
	if (std::isfinite(t) && std::isfinite(d))
	{
		ret->x = lineOrigin->x + t * lineDirection->x;
		ret->y = lineOrigin->y + t * lineDirection->y;
		ret->z = lineOrigin->z + t * lineDirection->z;
		return true;
	}
	else
	{
		return false;
	}
}

bool Engine::LineTriangleIntersection(const Triangle* t, const ZobVector3* l0, const ZobVector3* lv, ZobVector3& intersectionPt )
{
	//int
	//	intersect3D_RayTriangle(Ray R, Triangle T, Point * I)
	//{
		ZobVector3    u, v, n;              // triangle vectors
		ZobVector3    dir, w0, w;           // ray vectors
		float     r, a, b;              // params to calc ray-plane intersect

		// get triangle edge vectors and plane normal
		u.x = t->vb->x - t->va->x;
		u.y = t->vb->y - t->va->y;
		u.z = t->vb->z - t->va->z;
		v.x = t->vc->x - t->va->x;
		v.y = t->vc->y - t->va->y;
		v.z = t->vc->z - t->va->z;

		n = u * v;              // cross product
		//if (n == (ZobVector3)0)             // triangle is degenerate
		//	return -1;                  // do not deal with this case

		dir = lv;              // ray direction vector
		w0.x = l0->x - t->va->x;
		w0.x = l0->y - t->va->y;
		w0.x = l0->z - t->va->z;
		a = -ZobVector3::Dot(&n, &w0);
		b = ZobVector3::Dot(&n, &dir);
		if (fabs(b) < 0.00000001) {     // ray is  parallel to triangle plane
			if (a == 0)                 // ray lies in triangle plane
				return true;
			else return false;              // ray disjoint from plane
		}

		// get intersect point of ray with triangle plane
		r = a / b;
		if (r < 0.0)                    // ray goes away from triangle
			return false;                   // => no intersect
		// for a segment, also test if (r > 1.0) => no intersect

		intersectionPt = l0 + dir * r;            // intersect point of ray and plane

		// is I inside T?
		float    uu, uv, vv, wu, wv, D;
		uu = ZobVector3::Dot(&u, &u);
		uv = ZobVector3::Dot(&u, &v);
		vv = ZobVector3::Dot(&v, &v);
		w = intersectionPt - t->va;
		wu = ZobVector3::Dot(&w, &u);
		wv = ZobVector3::Dot(&w, &v);
		D = uv * uv - uu * vv;

		// get and test parametric coords
		float ss, tt;
		ss = (uv * wv - vv * wu) / D;
		if (ss < 0.0 || ss > 1.0)         // I is outside T
			return 0;
		tt = (uv * wu - uu * wv) / D;
		if (tt < 0.0 || (ss + tt) > 1.0)  // I is outside T
			return false;

		return true;                       // I is in T
}

ZobObject* Engine::GetObjectAt2DCoords(float x, float y)
{
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		Ray ray = c->From2DToWorld(x, y);
		int  idx = (int)floor((y + 1.0f) / 2.0f * m_nbRasterizers);
		Rasterizer* rast = m_rasterizers[idx];
		ZobVector3 inter;
		for (int i = 0; i < rast->GetNbTriangle(); i++)
		{
			const Triangle* t = rast->GetTriangle(i);
			if (this->LineTriangleIntersection(t, &ray.p, &ray.n, inter))
			{
				return rast->GetTriangle(i)->zobObject;
			}
		}
	}
	return NULL;
}

void Engine::ClipSegmentToPlane(ZobVector3 &s0, ZobVector3 &s1, ZobVector3 &pp, ZobVector3 &pn)
{
	ZobVector3 u = s1 - s0;
	ZobVector3 w = s0 - pp;
	float D = ZobVector3::Dot(&pn, &u);
	float N = -ZobVector3::Dot(&pn, &w);
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

void Engine::QueueSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ)
{
	QueuePartialSphere(camera, mat, radius, c, bold, noZ, 0, M_PI);
}

void Engine::QueuePartialSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold, bool noZ, float from, float to)
{
	static const int segs = 10;
	ZobVector3 v[segs+1][segs+1];
	for (int i = 0; i <= segs; i++)
	{
		for (int j = 0; j <= segs; j++)
		{
			float lon = (float)i / (float)segs;
			float lat = (float)j / (float)segs;
			v[i][j].x = sin(from + (to - from) * lat) * cos(2*M_PI*lon) * radius;
			v[i][j].z = sin(from + (to - from) * lat) * sin(2*M_PI*lon) * radius;
			v[i][j].y = cos(from + (to - from) * lat) * radius;
			mat->Mul(&v[i][j]);
		}
	}
	for (int i = 0; i < segs; i++)
	{
		for (int j = 0; j < segs; j++)
		{
			QueueLine(camera, &v[i][j], &v[(i+1)%segs][j], c, bold, noZ);
			//if (j < segs - 1)
			{
				QueueLine(camera, &v[i][j], &v[i][j + 1], c, bold, noZ);
			}
		}
	}
}

void Engine::QueueCapsule(const Camera* camera, const ZobMatrix4x4* mat, float radius, float height, const ZobVector3* dir, const uint c, bool bold, bool noZ)
{
	ZobMatrix4x4 m1;
	ZobMatrix4x4 m2;
	ZobVector3 zDir;
	m1 = ZobMatrix4x4(mat);
	m2 = ZobMatrix4x4(mat);
	zDir = dir;
	zDir.Mul(height/2.0f);
	m1.AddTranslation(zDir);
	zDir.Mul(-1.0f);
	m2.AddTranslation(zDir);
	QueuePartialSphere(camera, &m1, radius, c, bold, noZ, 0, M_PI / 2.0f);
	QueuePartialSphere(camera, &m2, radius, c, bold, noZ, M_PI/2.0f, M_PI);
	static const int segs = 10;
	ZobVector3 v1;
	ZobVector3 v2;
	for (int i = 0; i <= segs; i++)
	{
			float lon = (float)i / (float)segs;
			v1.x = cos(2 * M_PI * lon) * radius;
			v1.z = sin(2 * M_PI * lon) * radius;
			v1.y = 0.0f;
			v2.Copy(&v1);
			m1.Mul(&v1);
			m2.Mul(&v2);
			QueueLine(camera, &v1, &v2, c, bold, noZ);
	}
}

void Engine::QueueMesh(const Camera* camera, const ZobMatrix4x4* mat, ZobVector3* points, int width, int height, const uint c, bool bold)
{

}

void Engine::QueueBox(const Camera* camera, const ZobMatrix4x4* mat, const ZobVector3* halfExtends, const ZobVector3* pivot, const uint c, bool bold, bool noZ)
{
	ZobVector3 v0 = ZobVector3(-halfExtends->x, -halfExtends->y, -halfExtends->z);// +pivot;
	ZobVector3 v1 = ZobVector3(-halfExtends->x, halfExtends->y, -halfExtends->z);// + pivot;
	ZobVector3 v2 = ZobVector3(halfExtends->x, halfExtends->y, -halfExtends->z);// + pivot;
	ZobVector3 v3 = ZobVector3(halfExtends->x, -halfExtends->y, -halfExtends->z);// + pivot;

	ZobVector3 v4 = ZobVector3(-halfExtends->x, -halfExtends->y, halfExtends->z);// + pivot;
	ZobVector3 v5 = ZobVector3(-halfExtends->x, halfExtends->y, halfExtends->z);// + pivot;
	ZobVector3 v6 = ZobVector3(halfExtends->x, halfExtends->y, halfExtends->z);// + pivot;
	ZobVector3 v7 = ZobVector3(halfExtends->x, -halfExtends->y, halfExtends->z);// + pivot;

	mat->Mul(&v0);
	mat->Mul(&v1);
	mat->Mul(&v2);
	mat->Mul(&v3);
	mat->Mul(&v4);
	mat->Mul(&v5);
	mat->Mul(&v6);
	mat->Mul(&v7);
	QueueLine(camera, &v0, &v1, c, bold, noZ);
	QueueLine(camera, &v1, &v2, c, bold, noZ);
	QueueLine(camera, &v2, &v3, c, bold, noZ);
	QueueLine(camera, &v3, &v0, c, bold, noZ);
									
	QueueLine(camera, &v4, &v5, c, bold, noZ);
	QueueLine(camera, &v5, &v6, c, bold, noZ);
	QueueLine(camera, &v6, &v7, c, bold, noZ);
	QueueLine(camera, &v7, &v4, c, bold, noZ);
									
	QueueLine(camera, &v1, &v5, c, bold, noZ);
	QueueLine(camera, &v2, &v6, c, bold, noZ);
	QueueLine(camera, &v3, &v7, c, bold, noZ);
	QueueLine(camera, &v0, &v4, c, bold, noZ);
}									 

void Engine::QueueEllipse(const Camera* camera, const ZobVector3* center, const ZobVector3* vectorUp, const float r1, const float r2, const uint c, bool bold, bool noZ)
{
	int segs = 20;
	float r = 0.0f;
	float rot = (2.0f * M_PI) / (float)segs;
	ZobVector3 a;
	ZobMatrix4x4 m;
	ZobVector3 up = vectorUp;
	ZobVector3 baseUp = ZobVector3(0, 1, 0);
	ZobVector3 left = ZobVector3(1, 0, 0);
	ZobVector3 forward = ZobVector3(0, 0, 1);
	if (up != baseUp)
	{
		left = ZobVector3::Cross(&up, &baseUp);
	}
	//if (up != &left)
	{
		forward = ZobVector3::Cross(&left, &up);
	}
	left.Normalize();
	up.Normalize();
	forward.Normalize();
	m.FromVectors(left, up, forward);
	a = ZobVector3(r1 * cos(r), 0, r2 * sin(r));
	m.Mul(&a);
	a = a + center;
	ZobVector3 b = a;
	for(int i=1; i<=segs; i++)
	{
		r = rot * i;
		b = a;
		a = ZobVector3(r1 * cos(r), 0, r2 * sin(r));
		m.Mul(&a);
		a = a + center;
		QueueLine(camera, &a, &b, c, bold, noZ);
	}

}

void Engine::QueueLine(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const uint c, bool bold, bool noZ)
{
	if (m_started)
	{
		ZobVector3 a = ZobVector3(v1);
		ZobVector3 b = ZobVector3(v2);
		float outP2factor = 0.0f;
		bool bClipped = camera->ClipSegmentToFrustrum(&a, &b, outP2factor);
		if (bClipped)
		{
			if (m_lineQueueSize < m_maxLineQueueSize)
			{
				Line3D* l = &m_LineQueue[m_lineQueueSize];
				float za, zb = 0.0f;
				camera->GetViewMatrix()->Mul(&a);
				za = a.z;
				camera->GetProjectionMatrix()->Mul(&a);
				camera->GetViewMatrix()->Mul(&b);
				zb = b.z;
				camera->GetProjectionMatrix()->Mul(&b);
				a.x /= a.w;
				a.y /= a.w;
				a.z /= a.w;
				a.w = 1.0f;
				b.x /= b.w;
				b.y /= b.w;
				b.z /= b.w;
				b.w = 1.0f;
				a.x = (a.x + 1) * m_bufferData.width / 2.0f;
				a.y = (a.y + 1) * m_bufferData.height / 2.0f;
				b.x = (b.x + 1) * m_bufferData.width / 2.0f;
				b.y = (b.y + 1) * m_bufferData.height / 2.0f;
				l->xa = a.x;
				l->xb = b.x;
				l->ya = a.y;
				l->yb = b.y;
				l->za = za;
				l->zb = zb;
				l->c = c;
				l->bold = bold;
				l->noZ = noZ;
				int min = std::min<int>(a.y, b.y);
				int max = std::max<int>(a.y, b.y);
				min = clamp2(min, 0, (int)m_bufferData.height - 1);
				max = clamp2(max, 0, (int)m_bufferData.height - 1);
				min = min / m_rasterizerHeight;
				max = max / m_rasterizerHeight;
				QueueLineInRasters(l);
				m_lineQueueSize++;
			}
		}
	}
}

void Engine::CopyBuffer(uint* source, uint* dest)
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

void Engine::QueueLineInRasters(const Line3D* l) const
{
	int min = std::min<int>(l->ya, l->yb);
	int max = std::max<int>(l->ya, l->yb);
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
		m_rasterizers[i]->QueueLine(l);
	}
}

void Engine::QueueTriangleInRasters(const Triangle* t) const
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
		m_rasterizers[i]->QueueTriangle(t);
	}
}

void Engine::QueueTriangle(const Camera* c, const Triangle* t)
{
	if (m_started)
	{
		uint j = m_TriangleQueueSize;
		if (j < m_maxTrianglesQueueSize)
		{
			if (t->clipMode > Triangle::eClip_0_in)
			{
				if (t->clipMode < Triangle::eClip_2_in)
				{
					if (t->clipMode == Triangle::eClip_A_in_BC_out)
					{
						int nbNewTriangles = ClipTriangle(c, t);
						QueueTriangleInRasters(&m_TrianglesQueue[j]);
						m_TriangleQueueSize += nbNewTriangles;
						m_drawnTriangles += nbNewTriangles;
					}
					else if (t->clipMode == Triangle::eClip_B_in_AC_out)
					{
						int nbNewTriangles = ClipTriangle(c, t);
						QueueTriangleInRasters(&m_TrianglesQueue[j]);
						m_TriangleQueueSize += nbNewTriangles;
						m_drawnTriangles += nbNewTriangles;
					}
					else if (t->clipMode == Triangle::eClip_C_in_AB_out)
					{
						int nbNewTriangles = ClipTriangle(c, t);
						QueueTriangleInRasters(&m_TrianglesQueue[j]);
						m_TriangleQueueSize+=nbNewTriangles;
						m_drawnTriangles+= nbNewTriangles;
					}
						
				}
				else if (t->clipMode < Triangle::eClip_3_in)
				{
					int nbNewTriangles = SubDivideClippedTriangle(c, t);
					QueueTriangleInRasters(&m_TrianglesQueue[j]);
					QueueTriangleInRasters(&m_TrianglesQueue[j+1]);
					QueueTriangleInRasters(&m_TrianglesQueue[j+2]);
					QueueTriangleInRasters(&m_TrianglesQueue[j+3]);
					m_TriangleQueueSize += nbNewTriangles;
					m_drawnTriangles += nbNewTriangles;				
				}
				else
				{
					//memcpy(&m_TrianglesQueue[i][j], t, sizeof(Triangle));
					Triangle::CopyTriangle(&m_TrianglesQueue[j], t);
					RecomputeTriangleProj(c, &m_TrianglesQueue[j]);
					QueueTriangleInRasters(&m_TrianglesQueue[j]);
					m_TriangleQueueSize++;
					m_drawnTriangles++;
				}
			}
		}
	}
}

uint Engine::ClipTriangle(const Camera* c, const Triangle* t)
{
	uint j;
	j = m_TriangleQueueSize;
	if (j < m_maxTrianglesQueueSize)
	{
		Triangle* nt = &m_TrianglesQueue[j];
		Triangle::CopyTriangle(nt, t);
		float outP2factor = 0.0f;
		if (t->clipMode == Triangle::eClip_A_in_BC_out)
		{
			c->ClipSegmentToFrustrum(nt->va, nt->vb, outP2factor);
			RecomputeUv(nt->ua, nt->ub, outP2factor);
			c->ClipSegmentToFrustrum(nt->va, nt->vc, outP2factor);
			RecomputeUv(nt->ua, nt->uc, outP2factor);
		}
		else if (t->clipMode == Triangle::eClip_B_in_AC_out)
		{
			c->ClipSegmentToFrustrum(nt->vb, nt->va, outP2factor);
			RecomputeUv(nt->ub, nt->ua, outP2factor);
			c->ClipSegmentToFrustrum(nt->vb, nt->vc, outP2factor);
			RecomputeUv(nt->ub, nt->uc, outP2factor);
		}
		else //eClip_C_in_AB_out
		{
			c->ClipSegmentToFrustrum(nt->vc, nt->va, outP2factor);
			RecomputeUv(nt->uc, nt->ua, outP2factor);
			c->ClipSegmentToFrustrum(nt->vc, nt->vb, outP2factor); 
			RecomputeUv(nt->uc, nt->ub, outP2factor);
		}
		RecomputeTriangleProj(c, nt);
		return 1;
	}
	return 0;
}

uint Engine::SubDivideClippedTriangle(const Camera* c, const Triangle* t)
{
	int nbDrawn = 0;
	ZobVector3 pIn1;
	ZobVector2 pIn1Uv;
	ZobVector3 pIn2;
	ZobVector2 pIn2Uv;
	ZobVector3 pOut1;
	ZobVector2 pOut1Uv;
	ZobVector3 pOut2;
	ZobVector2 pOut2Uv;
	ZobVector3 pi;
	ZobVector2 piUv;
	if (t->clipMode == Triangle::eClip_AB_in_C_out)
	{
		pIn1 = t->va;
		pIn1Uv = t->ua;
		pIn2 = t->vb;
		pIn2Uv = t->ub;
		pOut1 = t->vc;
		pOut1Uv = t->uc;
	}
	else if (t->clipMode == Triangle::eClip_AC_in_B_out)
	{
		pIn1 = t->va;
		pIn1Uv = t->ua;
		pIn2 = t->vc;
		pIn2Uv = t->uc;
		pOut1 = t->vb;
		pOut1Uv = t->ub;
	}
	else if(Triangle::eClip_BC_in_A_out)
	{
		pOut1 = t->va;
		pOut1Uv = t->ua;
		pIn1 = t->vc;
		pIn1Uv = t->uc;
		pIn2 = t->vb;
		pIn2Uv = t->ub;
	}
	else
	{
		//?
		return 0;
	}
	pOut2 = pOut1;
	pOut2Uv = pOut1Uv;
	float outP2Factor = 0.0f;
	c->ClipSegmentToFrustrum(&pIn1, &pOut1, outP2Factor);
	RecomputeUv(&pIn1Uv, &pOut1Uv, outP2Factor);
	c->ClipSegmentToFrustrum(&pIn2, &pOut2, outP2Factor);
	RecomputeUv(&pIn2Uv, &pOut2Uv, outP2Factor);
	uint j;
	j = m_TriangleQueueSize + nbDrawn;
	if (j < m_maxTrianglesQueueSize)
	{
		Triangle* nt = &m_TrianglesQueue[j];
		Triangle::CopyTriangle(nt, t);
		nt->va->Copy(&pIn1);
		nt->ua->Copy(&pIn1Uv);
		nt->vb->Copy(&pOut2);
		nt->ub->Copy(&pOut2Uv);
		nt->vc->Copy(&pIn2);
		nt->uc->Copy(&pIn2Uv);
		RecomputeTriangleProj(c, nt);
		nbDrawn++;
	}
	j = m_TriangleQueueSize + nbDrawn;
	if (j < m_maxTrianglesQueueSize)
	{
		Triangle* nt = &m_TrianglesQueue[j];
		Triangle::CopyTriangle(nt, t);
		nt->va->Copy(&pIn1);
		nt->ua->Copy(&pIn1Uv);
		nt->vb->Copy(&pOut1);
		nt->ub->Copy(&pOut1Uv);
		nt->vc->Copy(&pOut2);
		nt->uc->Copy(&pOut2Uv);
		RecomputeTriangleProj(c, nt);
		nbDrawn++;
	}
	return nbDrawn;
}

void Engine::RecomputeTriangleProj(const Camera* c, Triangle* t)
{
	t->draw = true;
	t->clipMode = Triangle::eClip_3_in;
	t->pa->x = t->va->x;
	t->pa->y = t->va->y;
	t->pa->z = t->va->z;
	t->pa->w = t->va->w;
	t->pb->x = t->vb->x;
	t->pb->y = t->vb->y;
	t->pb->z = t->vb->z;
	t->pb->w = t->vb->w;
	t->pc->x = t->vc->x;
	t->pc->y = t->vc->y;
	t->pc->z = t->vc->z;
	t->pc->w = t->vc->w;
	BufferData* bData = GetBufferData();
	const ZobMatrix4x4* view = c->GetViewMatrix();
	const ZobMatrix4x4* proj = c->GetProjectionMatrix();
	float w = (float)bData->width / 2.0f;
	float h = (float)bData->height / 2.0f;
	view->Mul(t->pa);
	view->Mul(t->pb);
	view->Mul(t->pc);
	proj->Mul(t->pa);
	proj->Mul(t->pb);
	proj->Mul(t->pc);
	t->pa->x = (t->pa->x / t->pa->z + 1) * w;
	t->pa->y = (t->pa->y / t->pa->z + 1) * h;
	t->pb->x = (t->pb->x / t->pb->z + 1) * w;
	t->pb->y = (t->pb->y / t->pb->z + 1) * h;
	t->pc->x = (t->pc->x / t->pc->z + 1) * w;
	t->pc->y = (t->pc->y / t->pc->z + 1) * h;
	t->ComputeArea();
	if (t->options->cullMode == eCullMode_CounterClockwiseFace)
	{
		t->area = -t->area;
	}
	//t->material = NULL;
}

bool Engine::LineLineIntersection(const ZobVector3* a0, const ZobVector3* a1, const ZobVector3* b0, const ZobVector3* b1, ZobVector3& out, float &outFactor) const
{
	ZobVector3 da = ZobVector3(a1->x - a0->x, a1->y - a0->y, a1->z - a0->z);
	ZobVector3 db = ZobVector3(b1->x - b0->x, b1->y - b0->y, b1->z - b0->z);
	ZobVector3 dc = ZobVector3(b0->x - a0->x, b0->y - a0->y, b0->z - a0->z);
	ZobVector3 cross = ZobVector3::Cross(&da, &db);
	if (ZobVector3::Dot(&dc, &cross) != 0.0)
	{
		//return false;
	}
	ZobVector3 crossA = ZobVector3::Cross(&dc, &db);
	ZobVector3 crossB = ZobVector3::Cross(&da, &db);
	float s = ZobVector3::Dot(&crossA, &crossB) / crossB.length2();
	if (s >= 0.0 && s <= 1.0)
	{
		out = da;
		out.Mul(s);
		out.Add(a0);
		outFactor = s;
		return true;
	}
	return false;
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
bool Engine::GetProjectedCoords(ZobVector3* worldPos)
{
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		c->ProjectPointFromWorld(worldPos);
		return true;
	}
	return false;
}

float Engine::GetDistanceToCamera(ZobVector3* worldPos)
{
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		ZobVector3 v = c->GetWorldPosition();
		v = v - worldPos;
		return v.sqrtLength();
	}
	return 0.0f;
}