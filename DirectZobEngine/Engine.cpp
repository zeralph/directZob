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
#include "Camera.h"

#define MAX_TRIANGLES_PER_IMAGE 400000

static std::mutex m_mutex;
static std::condition_variable m_cond;

using namespace Core;
Engine::Engine(int width, int height, Events* events)
{
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
	//m_nbRasterizers = 1;
	m_triangleQueueSize = 50000;// MAX_TRIANGLES_PER_IMAGE / m_nbRasterizers;
	m_renderOutput = eRenderOutput_render;
	m_events = events;
	m_currentFrame = 0;
	m_zNear = Z_NEAR;
	m_zFar = Z_FAR;
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
	m_rasterTriangleQueues = (Triangle **)malloc(sizeof(Triangle) * m_nbRasterizers);
	m_rasterNbTriangleQueues = (long *)malloc(sizeof(long) * m_nbRasterizers);
	m_rasterLineQueues = new std::vector<Line3D>[m_nbRasterizers];
	//m_rasterizers = new std::vector<Rasterizer*>[m_nbRasterizers];
	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	m_rasterizerHeight = ceil((float)height / (float)m_nbRasterizers);
	int h0 = 0;
	
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer *r = new Rasterizer(width, h0, h0 + m_rasterizerHeight, &m_bufferData);
		m_rasterizers[i] = r;
		m_rasterTriangleQueues[i] = (Triangle*)malloc(sizeof(Triangle) * m_triangleQueueSize);
		m_rasterLineQueues[i].clear();
		h0 += m_rasterizerHeight;
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
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		delete m_rasterizers[i];
		delete m_rasterTriangleQueues[i];
	}
	delete m_rasterTriangleQueues;
	delete m_rasterNbTriangleQueues;
	delete m_rasterLineQueues;
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
		//m_rasterTriangleQueues[i].clear();
		m_rasterLineQueues[i].clear();
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
			m_zBuffer[i] = -1.0f;
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
				m_zBuffer[i + s] = -1.0f;
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
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Start(m_rasterTriangleQueues[i], m_rasterNbTriangleQueues[i], &m_rasterLineQueues[i], m_wireFrame, m_renderMode, m_currentFrame % 2, m_lightingPrecision);
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
		//m_rasterTriangleQueues[i]->clear();
		m_rasterLineQueues[i].clear();
		m_rasterLineQueues[i].shrink_to_fit();
		m_rasterNbTriangleQueues[i] = 0;
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

	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3X, 0xFF0000, true);
	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Y, 0x00FF00, true);
	QueueLine(camera, &ZobVector3::Vector3Zero, &ZobVector3::Vector3Z, 0x0000FF, true);
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
		Camera::Ray ray = c->From2DToWorld(x, y);
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

void Engine::QueueSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const uint c, bool bold)
{
	static const int segs = 10;
	ZobVector3 v[segs+1][segs+1];
	for (int i = 0; i <= segs; i++)
	{
		for (int j = 0; j <= segs; j++)
		{
			float lon = (float)i / (float)segs;
			float lat = (float)j / (float)segs;
			v[i][j].x = sin(M_PI * lat) * cos(2*M_PI*lon) * radius;
			v[i][j].y = sin(M_PI * lat) * sin(2*M_PI*lon) * radius;
			v[i][j].z = cos(M_PI *lat) * radius;
			mat->Mul(&v[i][j]);
		}
	}
	for (int i = 0; i < segs; i++)
	{
		for (int j = 0; j < segs; j++)
		{
			QueueLine(camera, &v[i][j], &v[(i+1)%segs][j], c, bold);
			//if (j < segs - 1)
			{
				QueueLine(camera, &v[i][j], &v[i][j + 1], c, bold);
			}
		}
	}
}

void Engine::QueueCapsule(const Camera* camera, const ZobMatrix4x4* mat, float radius, float height, const uint c, bool bold)
{

}

void Engine::QueueMesh(const Camera* camera, const ZobMatrix4x4* mat, ZobVector3* points, int width, int height, const uint c, bool bold)
{

}

void Engine::QueueBox(const Camera* camera, const ZobMatrix4x4* mat, const ZobVector3* halfExtends, const ZobVector3* pivot, const uint c, bool bold)
{
	ZobVector3 v0 = ZobVector3(-halfExtends->x, -halfExtends->y, -halfExtends->z) + pivot;
	ZobVector3 v1 = ZobVector3(-halfExtends->x, halfExtends->y, -halfExtends->z) + pivot;
	ZobVector3 v2 = ZobVector3(halfExtends->x, halfExtends->y, -halfExtends->z) + pivot;
	ZobVector3 v3 = ZobVector3(halfExtends->x, -halfExtends->y, -halfExtends->z) + pivot;

	ZobVector3 v4 = ZobVector3(-halfExtends->x, -halfExtends->y, halfExtends->z) + pivot;
	ZobVector3 v5 = ZobVector3(-halfExtends->x, halfExtends->y, halfExtends->z) + pivot;
	ZobVector3 v6 = ZobVector3(halfExtends->x, halfExtends->y, halfExtends->z) + pivot;
	ZobVector3 v7 = ZobVector3(halfExtends->x, -halfExtends->y, halfExtends->z) + pivot;

	mat->Mul(&v0);
	mat->Mul(&v1);
	mat->Mul(&v2);
	mat->Mul(&v3);
	mat->Mul(&v4);
	mat->Mul(&v5);
	mat->Mul(&v6);
	mat->Mul(&v7);
	QueueLine(camera, &v0, &v1, c, true);
	QueueLine(camera, &v1, &v2, c, true);
	QueueLine(camera, &v2, &v3, c, true);
	QueueLine(camera, &v3, &v0, c, true);
								
	QueueLine(camera, &v4, &v5, c, true);
	QueueLine(camera, &v5, &v6, c, true);
	QueueLine(camera, &v6, &v7, c, true);
	QueueLine(camera, &v7, &v4, c, true);
								
	QueueLine(camera, &v1, &v5, c, true);
	QueueLine(camera, &v2, &v6, c, true);
	QueueLine(camera, &v3, &v7, c, true);
	QueueLine(camera, &v0, &v4, c, true);
}

void Engine::QueueEllipse(const Camera* camera, const ZobVector3* center, const ZobVector3* vectorUp, const float r1, const float r2, const uint c, bool bold)
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
		QueueLine(camera, &a, &b, c, bold);
	}

}

void Engine::QueueLine(const Camera *camera, const ZobVector3 *v1, const ZobVector3 *v2, const uint c, bool bold)
{
	if (m_started)
	{
		ZobVector3 a = ZobVector3(v1);
		ZobVector3 b = ZobVector3(v2);
		float za, zb = 0.0f;
		camera->GetViewMatrix()->Mul(&a);
		//camera->ToViewSpace(&a);
		za = a.z;
		camera->GetProjectionMatrix()->Mul(&a);
		camera->GetViewMatrix()->Mul(&b);
		//camera->ToViewSpace(&b);
		zb = b.z;
		camera->GetProjectionMatrix()->Mul(&b);

		if (a.z < m_bufferData.zNear && b.z < m_bufferData.zNear)
		{
			return;
		}
		else if (a.z < m_bufferData.zNear || b.z < m_bufferData.zNear)
		{
			ZobVector3 pp = ZobVector3(0, 0, m_bufferData.zNear);
			ZobVector3 pn = ZobVector3(0, 0, 1);
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
			l.bold = false;
			int min = std::min<int>(a.y, b.y);
			int max = std::max<int>(a.y, b.y);
			min = clamp2(min, 0, (int)m_bufferData.height - 1);
			max = clamp2(max, 0, (int)m_bufferData.height - 1);
			min =  min / m_rasterizerHeight;
			max =  max / m_rasterizerHeight;
//			min = 0;
//			max = m_nbRasterizers-1;
			if(isinf(l.xa) || isinf(l.xb) || isinf(l.ya) || isinf(l.yb))
			{
				return;
			}
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

bool Engine::ClipSegment(ZobVector3 *a, ZobVector3 *b)
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
		ZobVector3 v = ZobVector3(c->GetPosition());
		v = v - worldPos;
		return v.sqrtLength();
	}
	return 0.0f;
}
