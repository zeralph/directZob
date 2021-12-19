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
#include <mutex>
#include "Behaviors/ZobBehaviorMesh.h"
#undef None
#include "../../dependencies/optick/include/optick.h"

#define MAX_TRIANGLES_PER_IMAGE 400000
#define MAX_RENDER_OPTIONS 1000

static ZobVector3 red(1, 0, 0);
static ZobVector3 green(0, 1, 0);
static ZobVector3 blue(0, 0, 1);
static ZobVector3 black(0, 0, 0);
static ZobVector3 yellow(1, 1, 0);

std::condition_variable** m_conditionvariables;
std::mutex** m_mutexes;

Engine::Engine(int width, int height, Events* events)
{
	m_started = false;
	m_wireFrame = false;
	m_rasterizerHeight = 0;
	m_showNormals = false;
	m_showGrid = false;
	m_drawGizmos = false;
	m_showBBoxes = false;
	m_doResize = false;
	m_drawPhysicsGizmos = false;
	m_lockFrustrum = false;
	m_drawZobObjectGizmos = true;
	m_drawCameraGizmos = false;
	m_showText = true;
	m_nbRasterizers = std::thread::hardware_concurrency();
	m_EqualizeTriangleQueues = false;
	m_perspCorrection = true;
	m_nbBitsPerColorDepth = eBitsPerColor_full;	//
	m_varExposer = new ZobVariablesExposer(0);
	m_dithering = false;
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
	m_nbRasterizers --;
	m_nbRasterizers = max(m_nbRasterizers, (uint)1);
	//m_nbRasterizers = 4;
	m_maxTrianglesQueueSize = 200000;// MAX_TRIANGLES_PER_IMAGE / m_nbRasterizers;
	m_maxLineQueueSize = 200000;
	m_events = events;
	m_currentFrame = 0;
	m_zNear = 0.11f;
	m_zFar = 1000.0f;
	m_currentBuffer = 0;
	m_buffer = (uint**)malloc(sizeof(uint*) * 2);
	m_zBuffer = (float**)malloc(sizeof(float*) * 2);
	for (int i = 0; i < 2; i++)
	{
		m_buffer[i] = (uint*)malloc(sizeof(uint) * width * height);
		m_zBuffer[i] = (float*)malloc(sizeof(float) * width * height);
	}
	m_bufferClearArray = (uint*)malloc(sizeof(uint) * width * height);
	m_zBufferClearArray = (float*)malloc(sizeof(float) * width * height);
	uint c = 0;// ZobColor(DirectZob::GetInstance()->GetLightManager()->GetClearColor()).GetRawValue();;
	for (int i = 0; i < width * height; i++)
	{
		m_bufferClearArray[i] = c;
		m_zBufferClearArray[i] = 100000.0f;
	}
	m_showZBuffer = false;
	m_nextWidth = width;
	m_nextHeight = height;
	m_bufferData.height = height;
	m_bufferData.width = width;
	m_bufferData.zNear = m_zNear;
	m_bufferData.zFar = m_zFar;
	m_bufferData.buffer = m_buffer[m_currentBuffer];
	m_bufferData.zBuffer = m_zBuffer[m_currentBuffer];
	m_bufferData.size = width * height;
	m_nbPixels = 0;

	m_rasterizers = (Rasterizer**)malloc(sizeof(Rasterizer) * m_nbRasterizers);
	m_conditionvariables = (std::condition_variable**)malloc(sizeof(std::condition_variable) * m_nbRasterizers);
	m_mutexes = (std::mutex**)malloc(sizeof(std::mutex) * m_nbRasterizers);
	m_renderOptions = (Triangle::RenderOptions*)malloc(sizeof(Triangle::RenderOptions) * MAX_RENDER_OPTIONS);
	m_rasterizerHeight = ceil((float)height / (float)m_nbRasterizers);
	int h0 = 0;
	int h1 = m_rasterizerHeight;
	int nbTrianglesPerRasterize = m_maxTrianglesQueueSize;// / m_nbRasterizers;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		Rasterizer* r = new Rasterizer(width, height, h0, h1, nbTrianglesPerRasterize, (i+1), &m_bufferData);
		m_rasterizers[i] = r;
		m_conditionvariables[i] = new std::condition_variable();
		m_mutexes[i] = new std::mutex();
		h0 += m_rasterizerHeight;
		h1 += m_rasterizerHeight;
		h1 = min(height, h1);
	}

	m_LineQueue = (Line3D*)malloc(sizeof(Line3D) * m_maxLineQueueSize);
	m_TrianglesQueue = (Triangle*)malloc(sizeof(Triangle) * m_maxTrianglesQueueSize);
	m_verticesData = (ZobVector3*)malloc(sizeof(ZobVector3) * m_maxTrianglesQueueSize * 10); // vertices plus normals plus ...
	m_uvData = (ZobVector2*)malloc(sizeof(ZobVector2) * m_maxTrianglesQueueSize * 3);
	m_colorData = (ZobColor*)malloc(sizeof(ZobColor) * m_maxTrianglesQueueSize * 3);
	ulong tot = sizeof(Triangle);
	tot = sizeof(Line3D)* m_maxLineQueueSize + sizeof(Triangle) * m_maxTrianglesQueueSize + sizeof(ZobVector3) * m_maxTrianglesQueueSize * 10 + sizeof(ZobVector2) * m_maxTrianglesQueueSize * 3;
	tot /= 1024*1024;

	int vertexIdx = 0;
	int uvIdx = 0;
	for (int i = 0; i < m_maxTrianglesQueueSize; i++)
	{
		m_TrianglesQueue[i].va = &m_verticesData[vertexIdx + 0];
		m_TrianglesQueue[i].vb = &m_verticesData[vertexIdx + 1];
		m_TrianglesQueue[i].vc = &m_verticesData[vertexIdx + 2];
		m_TrianglesQueue[i].pa = &m_verticesData[vertexIdx + 3];
		m_TrianglesQueue[i].pb = &m_verticesData[vertexIdx + 4];
		m_TrianglesQueue[i].pc = &m_verticesData[vertexIdx + 5]; 
		m_TrianglesQueue[i].na = &m_verticesData[vertexIdx + 6]; 
		m_TrianglesQueue[i].nb = &m_verticesData[vertexIdx + 7]; 
		m_TrianglesQueue[i].nc = &m_verticesData[vertexIdx + 8]; 
		m_TrianglesQueue[i].n  = &m_verticesData[vertexIdx + 9];
		m_TrianglesQueue[i].ua = &m_uvData[uvIdx + 0]; 
		m_TrianglesQueue[i].ub = &m_uvData[uvIdx + 1]; 
		m_TrianglesQueue[i].uc = &m_uvData[uvIdx + 2]; 
		m_TrianglesQueue[i].ca = &m_colorData[uvIdx + 0];
		m_TrianglesQueue[i].cb = &m_colorData[uvIdx + 1];
		m_TrianglesQueue[i].cc = &m_colorData[uvIdx + 2];
		m_TrianglesQueue[i].options = NULL;
		m_TrianglesQueue[i].material = NULL;
		m_TrianglesQueue[i].zobObject = NULL;
		m_TrianglesQueue[i].draw = false;
		m_TrianglesQueue[i].area = 0;
		m_TrianglesQueue[i].verticeAIndex = 0;
		m_TrianglesQueue[i].verticeBIndex = 0;
		m_TrianglesQueue[i].verticeCIndex = 0;
		vertexIdx += 10;
		uvIdx += 3;
	}
	m_lineQueueSize = 0;
	m_LastTriangleQueueSize = 0;
	m_TriangleQueueSize = 0;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Init(m_conditionvariables[i], m_mutexes[i]);
	}

	eRenderMode rm[3] = { eRenderMode::eRenderMode_fullframe, eRenderMode::eRenderMode_interlaced, eRenderMode::eRenderMode_scanline };
	const char* rmStr[3] = { "Fullframe", "Interlaced", "Scanline" };
	m_varExposer->WrapEnum<eRenderMode>("Render mode", &m_renderMode, 3, rm, rmStr, NULL, false, true);

	eLightingPrecision lm[3] = { eLightingPrecision::eLightingPrecision_noLighting, eLightingPrecision::eLightingPrecision_vertex, eLightingPrecision::eLightingPrecision_pixel };
	const char* lmStr[3] = { "No lighting", "Vertex", "Pixel" };
	m_varExposer->WrapEnum<eLightingPrecision>("Lighting mode", &m_lightingPrecision, 3, lm, lmStr, NULL, false, true);

	eBitsPerColor bp[9] = { eBitsPerColor::eBitsPerColor_full, eBitsPerColor::eBitsPerColor_1, eBitsPerColor::eBitsPerColor_2, eBitsPerColor::eBitsPerColor_3, 
	eBitsPerColor::eBitsPerColor_4, eBitsPerColor::eBitsPerColor_5, eBitsPerColor::eBitsPerColor_6, eBitsPerColor::eBitsPerColor_7, eBitsPerColor::eBitsPerColor_8,};
	const char* bpStr[9] = { "Full", "1", "2", "3", "4", "5", "6", "7", "8"};
	m_varExposer->WrapEnum<eBitsPerColor>("Bits per color", &m_nbBitsPerColorDepth, 9, bp, bpStr, NULL, false, true);
	m_varExposer->WrapVariable<bool>("Dithering", &m_dithering, NULL, false, true);
	m_varExposer->WrapVariable<float>("Z near", &m_zNear, NULL, false, true);
	m_varExposer->WrapVariable<float>("Z far", &m_zFar, NULL, false, true);

	m_varExposer->WrapVariable<bool>("Show Z Buffer", &m_showZBuffer, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Wireframe", &m_wireFrame, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Show Normals", &m_showNormals, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Show grid", &m_showGrid, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Draw gizmos", &m_drawGizmos, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Draw physics gizmos", &m_drawPhysicsGizmos, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Draw camera gizmos", &m_drawCameraGizmos, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Draw object gizmos", &m_drawZobObjectGizmos, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Show bounding boxes", &m_showBBoxes, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Show text", &m_showText, NULL, false, false);
	m_varExposer->WrapVariable<bool>("Equalize triangle queues", &m_EqualizeTriangleQueues, NULL, false, true);
	m_varExposer->WrapVariable<bool>("Perspective Correction", &m_perspCorrection, NULL, false, true);
	m_varExposer->WrapVariable<int>("Rastyerizer height", &m_rasterizerHeight, NULL, true, false);
	m_varExposer->WrapVariable<uint>("Number of rasterizers", &m_nbRasterizers, NULL, true, false);
	m_varExposer->WrapVariable<uint>("Number of drawn triangles", &m_drawnTriangles, NULL, true, false);
	m_varExposer->Load();
	std::string n = "Engine initialized with " + std::to_string(m_nbRasterizers) + " rasterizer(s) for " + std::to_string(m_maxTrianglesQueueSize) + " triangles per image";
	DirectZob::LogWarning(n.c_str());
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
StopRasterizers();
WaitForRasterizersEnd();
for (int i = 0; i < m_nbRasterizers; i++)
{
	//m_mutexes[i]->unlock();
	//m_conditionvariables[i]->notify_all();
	delete m_rasterizers[i];
	//delete m_conditionvariables[i];
	//delete m_mutexes[i];
}
free(m_TrianglesQueue);
free(m_verticesData);
free(m_uvData);
free(m_colorData);
free(m_rasterizers);
free(m_conditionvariables);
free(m_mutexes);
free(m_renderOptions);
m_events = NULL;
}

void Engine::Start()
{
	DirectZob::GetInstance()->GetZobObjectManager()->Init();
	m_varExposer->Load();
	for (int i = 0; i < m_nbRasterizers; i++)
		m_started = true;
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

bool Engine::Resize(int width, int height)
{
	if (m_nextWidth != width || m_nextHeight != height)
	{
		m_nextWidth = width;
		m_nextHeight = height;
		m_doResize = true;
		//return ResizeInternal();
	}
	return false;
}


bool Engine::ResizeInternal()
{
	if (m_doResize)
	{
		m_doResize = false;
		for (int i = 0; i < 2; i++)
		{
			free(m_buffer[i]);
			free(m_zBuffer[i]);
		}
		free(m_buffer);
		free(m_zBuffer);
		m_buffer = NULL;
		m_zBuffer = NULL;
		m_buffer = (uint**)malloc(sizeof(uint*) * 2);
		m_zBuffer = (float**)malloc(sizeof(float*) * 2);
		for (int i = 0; i < 2; i++)
		{
			m_buffer[i] = (uint*)malloc(sizeof(uint) * m_nextWidth * m_nextHeight);
			m_zBuffer[i] = (float*)malloc(sizeof(float) * m_nextWidth * m_nextHeight);
		}
		m_bufferData.height = m_nextHeight;
		m_bufferData.width = m_nextWidth;
		m_bufferData.buffer = m_buffer[m_currentBuffer];
		m_bufferData.zBuffer = m_zBuffer[m_currentBuffer];
		//m_bufferData.zNear = m_zNear;
		//m_bufferData.zFar = m_zFar;
		m_bufferData.size = m_nextWidth * m_nextHeight;
		ZobColor c = ZobColor(DirectZob::GetInstance()->GetLightManager()->GetClearColor());
		ClearBuffer(&c);
		m_rasterizerHeight = ceil((float)m_nextHeight / (float)m_nbRasterizers);
		for (int i = 0; i < m_nbRasterizers; i++)
		{
			m_rasterizers[i]->Resize(m_nextWidth, m_nextHeight);
		}
		return true;
	}
	return false;
}

void Engine::ClearBuffer(const ZobColor* color)
{
	OPTICK_EVENT();
	int oldBuffer = (m_currentBuffer + 1) % 2;
	uint v = color->GetRawValue();
	//if (v != m_buffer[oldBuffer][0])
	{
		for (int i = 0; i < m_bufferData.width * m_bufferData.height; i++)
		{
			m_buffer[oldBuffer][i] = v;
		}
	}
	if (m_renderMode == eRenderMode_fullframe )
	{
		//memset(m_buffer[oldBuffer], 0, sizeof(uint) * m_bufferData.width * m_bufferData.height);
		memset(m_zBuffer[oldBuffer], 0, sizeof(float) * m_bufferData.width * m_bufferData.height);
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
				m_zBuffer[oldBuffer][i + s] = 1.0f;
				m_buffer[oldBuffer][i + s] = c;
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
				m_zBuffer[oldBuffer][i+s] = -1.0f;
				m_buffer[oldBuffer][i+s] = v;
			}
		}
	}
	m_drawnTriangles = 0;
}

int Engine::StartDrawingScene()
{
	OPTICK_EVENT();
	ResizeInternal();
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	if (inputMap->GetBoolIsNew(ZobInputManager::SwitchBuffers))
	{
		ToggleZbufferOutput();
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::SwitchEqualizeTriangleQueues))
	{
		m_EqualizeTriangleQueues = !m_EqualizeTriangleQueues;
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::switchPerspectiveCorrection))
	{
		m_perspCorrection = !m_perspCorrection;
		EnablePerspectiveCorrection(m_perspCorrection);
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::switchColorDepth))
	{
		eBitsPerColor i = (eBitsPerColor)(m_nbBitsPerColorDepth + 1);
		if (i == __eBitsPerColor_MAX__)
		{
			i = (eBitsPerColor)0;
		}
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::NextLightMode))
	{
		eLightingPrecision i = GetLightingPrecision();
		i = (eLightingPrecision)(i+1);
		if (i == __eLightingPrecision_MAX__)
		{
			i = (eLightingPrecision)0;
		}
		SetLightingPrecision(i);
	}
	if (!m_started)
	{
		return 0;
	}
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Start();
		m_conditionvariables[i]->notify_one();
	}
	return 0;
}

void Engine::SwapBuffers()
{
	
	m_currentBuffer = (m_currentBuffer + 1) % 2;
	m_bufferData.buffer = m_buffer[m_currentBuffer];
	m_bufferData.zBuffer = m_zBuffer[m_currentBuffer];
	m_bufferData.curBuffer = m_currentBuffer;
}


int Engine::SetDisplayedBuffer()
{
	if (m_showZBuffer)
	{
		uint c;
		for (int i = 0; i < m_bufferData.size; i++)
		{
			c = (uint)((1.0f / m_zBuffer[m_currentBuffer][i]) * 255.0f);
			c = (c << 16) + (c << 8) + c;
			m_buffer[m_currentBuffer][i] = c;
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
	OPTICK_EVENT();
	m_LastTriangleQueueSize = m_TriangleQueueSize;
	m_lineQueueSize = 0;
	m_TriangleQueueSize = 0;
	m_usedRenderOptions = 0;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->Clear();
	}
}

void Engine::StopRasterizers()
{
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m_rasterizers[i]->End();
	}
}

float Engine::WaitForRasterizersEnd()
{
	OPTICK_CATEGORY("WaitForRasterizersEnd", Optick::Category::Wait);
	float t = 0.0f;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		if (m_rasterizers[i])
		{
			std::unique_lock<std::mutex> g(*m_mutexes[i]);
		}
	}
	return t;
}

ZobObject* Engine::GetObjectAt2DCoords(float x, float y, bool editorObjectsOnly)
{
	int  idx = (int)((y / (float)m_bufferData.height) * m_nbRasterizers);
	if (idx < 0 || idx >= m_nbRasterizers)
	{
		return NULL;
	}
	ZobObject* z = NULL;
	float minZ = m_bufferData.zFar;
	for (idx = 0; idx < m_nbRasterizers; idx++)
	{
		Rasterizer* rast = m_rasterizers[idx];
		ZobVector3 p = ZobVector3(x, y, 0);
		for (int i = 0; i < rast->GetNbTriangle(); i++)
		{
			const Triangle* t = rast->GetTriangle(i);
			if (!editorObjectsOnly || (t && t->zobObject && t->zobObject->IsEditorObject()))
			{
				if (t->PointInTriangle2D(&p) && t->zobObject && t->pa->z < minZ)
				{
					z = t->zobObject;
					minZ = t->pa->z;
				}
			}
		}
	}
	return z;
	/*
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
	if (c)
	{
		Ray ray = c->From2DToWorld(x, y);
		int  idx = (int)floor((y + 1.0f) / 2.0f * m_nbRasterizers);
		if (idx < 0 || idx >= m_nbRasterizers)
		{
			return NULL;
		}
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
	*/
}

void Engine::QueueSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const ZobColor* c, bool bold, bool noZ)
{
	QueuePartialSphere(camera, mat, radius, c, bold, noZ, 0, M_PI);
}

void Engine::QueuePartialSphere(const Camera* camera, const ZobMatrix4x4* mat, const float radius, const ZobColor* c, bool bold, bool noZ, float from, float to)
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

void Engine::QueueCapsule(const Camera* camera, const ZobMatrix4x4* mat, float radius, float height, const ZobVector3* dir, const ZobColor* c, bool bold, bool noZ)
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

void Engine::QueueMesh(const Camera* camera, const ZobMatrix4x4* mat, ZobVector3* points, int width, int height, const ZobColor* c, bool bold)
{

}

void Engine::QueueWorldBox(const Camera* camera, const Box* box, const ZobColor* c, bool bold, bool noZ)
{
	ZobVector3 p0, p1, p2, p3, p4, p5, p6, p7;
	p0 = box->p0;
	p1 = box->p1;
	p2 = box->p2;
	p3 = box->p3;
	p4 = box->p4;
	p5 = box->p5;
	p6 = box->p6;
	p7 = box->p7;

	QueueLine(camera, &p0, &p1, c, bold, noZ);
	QueueLine(camera, &p1, &p2, c, bold, noZ);
	QueueLine(camera, &p2, &p3, c, bold, noZ);
	QueueLine(camera, &p3, &p0, c, bold, noZ);
							 
	QueueLine(camera, &p4, &p5, c, bold, noZ);
	QueueLine(camera, &p5, &p6, c, bold, noZ);
	QueueLine(camera, &p6, &p7, c, bold, noZ);
	QueueLine(camera, &p7, &p4, c, bold, noZ);
							 
	QueueLine(camera, &p1, &p5, c, bold, noZ);
	QueueLine(camera, &p2, &p6, c, bold, noZ);
	QueueLine(camera, &p3, &p7, c, bold, noZ);
	QueueLine(camera, &p0, &p4, c, bold, noZ);
}

void Engine::QueueBox(const Camera* camera, const ZobMatrix4x4* mat, const ZobVector3* halfExtends, const ZobVector3* pivot, const ZobColor* c, bool bold, bool noZ)
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

void Engine::QueueEllipse(const Camera* camera, const ZobVector3* center, const ZobVector3* vectorUp, const float r1, const float r2, const ZobColor* c, bool bold, bool noZ)
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
		//QueueTriangle(camera, center, &a, &b, c, true, false);
	}
}

void Engine::QueueTriangle(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const ZobVector3* v3, const ZobColor* c, bool transparent, bool noZ)
{
	Triangle* t = &m_TrianglesQueue[m_TriangleQueueSize];
	t->va->x = v1->x;
	t->va->y = v1->y;
	t->va->z = v1->z;
	t->va->w = 1;
	t->vb->x = v2->x;
	t->vb->y = v2->y;
	t->vb->z = v2->z;
	t->vb->w = 1;
	t->vc->x = v3->x;
	t->vc->y = v3->y;
	t->vc->z = v3->z;
	t->vc->w = 1;
	t->ca->Copy(c);
	t->cb->Copy(c);
	t->cc->Copy(c);
	t->material = NULL;
	t->clipMode = Triangle::eClip_3_in;
	Triangle::RenderOptions* r = &m_renderOptions[m_usedRenderOptions];
	r->bTransparency = transparent;
	r->cullMode = Triangle::RenderOptions::eCullMode_None;
	r->zBuffered = !noZ;
	r->lightMode = Triangle::RenderOptions::eLightMode_none;
	t->options = r;
	m_usedRenderOptions++;
	RecomputeTriangleProj(camera, t);
	m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
	m_TriangleQueueSize++;
}

void Engine::QueueLine(const Camera* camera, const ZobVector3* v1, const ZobVector3* v2, const ZobColor* c, bool bold, bool noZ)
{
	if (m_started)
	{
		ZobVector3 a = ZobVector3(0,0,0);
		ZobVector3 b = ZobVector3(0,0,0);
		a.Copy(v1);
		b.Copy(v2);
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
				l->c = c->GetRawValue();
				l->bold = bold;
				l->noZ = noZ;
				int min = std::min<int>(a.y, b.y);
				int max = std::max<int>(a.y, b.y);
				min = CLAMP(min, 0, (int)m_bufferData.height - 1);
				max = CLAMP(max, 0, (int)m_bufferData.height - 1);
				min = min / m_rasterizerHeight;
				max = max / m_rasterizerHeight;
				QueueLineInRasters(l, m_lineQueueSize);
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

void Engine::QueueLineInRasters(const Line3D* l, int idx) const
{
	int i = idx % m_nbRasterizers;
	assert(i >= 0);
	assert(i < m_nbRasterizers);
	m_rasterizers[i]->QueueLine(l);
}

int Engine::QueueTriangleInRasters(const Triangle* t, int idx) const
{
	if (t->options->cullMode == Triangle::RenderOptions::eCullMode_CounterClockwiseFace && t->area >0 )
	{
		return 0;
	}
	if (t->options->cullMode == Triangle::RenderOptions::eCullMode_ClockwiseFace && t->area < 0)
	{
		return 0;
	}
	if (m_TriangleQueueSize < m_maxTrianglesQueueSize)
	{
		if (UsePerspectiveCorrection())
		{
			t->ua->x /= t->pa->z;
			t->ua->y /= t->pa->z;
			t->ub->x /= t->pb->z;
			t->ub->y /= t->pb->z;
			t->uc->x /= t->pc->z;
			t->uc->y /= t->pc->z;
		}
		if (!m_EqualizeTriangleQueues)
		{
			int min = (int)fmaxf(0, fminf(t->pa->y, fminf(t->pb->y, t->pc->y)));
			int max = (int)fminf(m_bufferData.height, fmaxf(t->pa->y, fmaxf(t->pb->y, t->pc->y)));
			min /= m_rasterizerHeight;
			max /= m_rasterizerHeight;
			min--;
			min = fmax(0, min);
			
			max = fmin(max, m_nbRasterizers-1);
			//min = max;
			for (int i = min; i <= max; i++)
			{
				m_rasterizers[i]->QueueTriangle(t);
			}
		}
		else
		{
			int i = (idx) % m_nbRasterizers;
			assert(i >= 0);
			assert(i < m_nbRasterizers);
			m_rasterizers[i]->QueueTriangle(t);
		}
	}
	return 1;
}

void Engine::QueueProjectedTriangle(const Camera* c, const Triangle* t)
{
	if (m_TriangleQueueSize < m_maxTrianglesQueueSize)
	{
		Triangle::CopyTriangle(&m_TrianglesQueue[m_TriangleQueueSize], t);
		//TODO : adjust to frustrums 
		m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
		m_TriangleQueueSize++;
	}
}

void Engine::QueueWorldTriangle(const Camera* c, const Triangle* t)
{
	if (m_started)
	{
		if (m_TriangleQueueSize < m_maxTrianglesQueueSize)
		{
			if (t->clipMode > Triangle::eClip_0_in)
			{
				if (t->clipMode < Triangle::eClip_2_in)
				{
					if (t->clipMode == Triangle::eClip_A_in_BC_out)
					{
						Triangle::CopyTriangle(&m_TrianglesQueue[m_TriangleQueueSize], t);
						ClipTriangle(c, &m_TrianglesQueue[m_TriangleQueueSize]);
						m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
						m_TriangleQueueSize ++;
					}
					else if (t->clipMode == Triangle::eClip_B_in_AC_out)
					{
						Triangle::CopyTriangle(&m_TrianglesQueue[m_TriangleQueueSize], t);
						ClipTriangle(c, &m_TrianglesQueue[m_TriangleQueueSize]);
						m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
						m_TriangleQueueSize ++;
					}
					else if (t->clipMode == Triangle::eClip_C_in_AB_out)
					{
						Triangle::CopyTriangle(&m_TrianglesQueue[m_TriangleQueueSize], t);
						ClipTriangle(c, &m_TrianglesQueue[m_TriangleQueueSize]);
						m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
						m_TriangleQueueSize++;
					}
						
				}
				else if (t->clipMode < Triangle::eClip_3_in)
				{
					SubDivideClippedTriangle(c, t);		
				}
				else 
				{
					//memcpy(&m_TrianglesQueue[i][j], t, sizeof(Triangle));
					Triangle::CopyTriangle(&m_TrianglesQueue[m_TriangleQueueSize], t);
					RecomputeTriangleProj(c, &m_TrianglesQueue[m_TriangleQueueSize]);
					m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
					m_TriangleQueueSize++;
				}
			}
		}
	}
}

uint Engine::ClipTriangle(const Camera* c, Triangle* t)
{
	Triangle* nt = t;// &m_TrianglesQueue[m_TriangleQueueSize];
	//Triangle::CopyTriangle(nt, t);
	float outP2factor = 0.0f;
	
	if (t->clipMode == Triangle::eClip_A_in_BC_out)
	{
		c->ClipSegmentToFrustrum(nt->va, nt->vb, outP2factor);
		RecomputeUv(nt->ua, nt->ub, outP2factor);
		RecomputeNormal(nt->na, nt->nb, outP2factor);
		RecomputeColor(nt->ca, nt->cb, outP2factor);
		c->ClipSegmentToFrustrum(nt->va, nt->vc, outP2factor);
		RecomputeUv(nt->ua, nt->uc, outP2factor);
		RecomputeNormal(nt->na, nt->nc, outP2factor);
		RecomputeColor(nt->ca, nt->cc, outP2factor);	
	}
	else if (t->clipMode == Triangle::eClip_B_in_AC_out)
	{
		c->ClipSegmentToFrustrum(nt->vb, nt->va, outP2factor);
		RecomputeUv(nt->ub, nt->ua, outP2factor);
		RecomputeNormal(nt->nb, nt->na, outP2factor);
		RecomputeColor(nt->cb, nt->ca, outP2factor);
		c->ClipSegmentToFrustrum(nt->vb, nt->vc, outP2factor);
		RecomputeUv(nt->ub, nt->uc, outP2factor);
		RecomputeNormal(nt->nb, nt->nc, outP2factor);
		RecomputeColor(nt->cb, nt->cc, outP2factor);
	}
	else if (t->clipMode == Triangle::eClip_C_in_AB_out)
	{
		c->ClipSegmentToFrustrum(nt->vc, nt->va, outP2factor);
		RecomputeUv(nt->uc, nt->ua, outP2factor);
		RecomputeNormal(nt->nc, nt->na, outP2factor);
		RecomputeColor(nt->cc, nt->ca, outP2factor);
		c->ClipSegmentToFrustrum(nt->vc, nt->vb, outP2factor); 
		RecomputeUv(nt->uc, nt->ub, outP2factor);
		RecomputeNormal(nt->nc, nt->nb, outP2factor);
		RecomputeColor(nt->cc, nt->cb, outP2factor);
	}
	else if (t->clipMode == Triangle::eClip_3_out_corner)
	{
		DirectZob::LogWarning("here");
	}
	else
	{
		//??
		int y = 0;
		y++;
	}
	RecomputeTriangleProj(c, nt);
	return 1;
}

uint Engine::SubDivideClippedTriangle(const Camera* c, const Triangle* t)
{
	ZobVector3 pIn1;
	ZobVector2 pIn1Uv;
	ZobColor pIn1Cv;
	ZobVector3 pIn1n;
	ZobVector3 pIn2;
	ZobVector2 pIn2Uv;
	ZobColor pIn2Cv;
	ZobVector3 pIn2n;
	ZobVector3 pOut1;
	ZobVector2 pOut1Uv;
	ZobColor pOut1Cv;
	ZobVector3 pOut1n;
	ZobVector3 pOut2;
	ZobVector2 pOut2Uv;
	ZobColor pOut2Cv;
	ZobVector3 pOut2n;
	ZobVector3 pi;
	ZobVector2 piUv;
	if (t->clipMode == Triangle::eClip_AB_in_C_out)
	{
		pIn1 = t->va;
		pIn1Uv = t->ua;
		pIn1Cv = t->ca;
		pIn1n = t->na;

		pIn2 = t->vb;
		pIn2Uv = t->ub;
		pIn2Cv = t->cb;
		pIn2n = t->nb;

		pOut1 = t->vc;
		pOut1Uv = t->uc;
		pOut1Cv = t->cc;
		pOut1n = t->n;
	}
	else if (t->clipMode == Triangle::eClip_AC_in_B_out)
	{
		//return 0;
		pIn1 = t->vc;
		pIn1Uv = t->uc;
		pIn1Cv = t->cc;
		pIn1n = t->nc;

		pIn2 = t->va;
		pIn2Uv = t->ua;
		pIn2Cv = t->ca;
		pIn2n = t->na;

		pOut1 = t->vb;
		pOut1Uv = t->ub;
		pOut1Cv = t->cb;
		pOut1n = t->nb;
	}
	else if(Triangle::eClip_BC_in_A_out)
	{
		//return 0;
		pIn1 = t->vb;
		pIn1Uv = t->ub;
		pIn1Cv = t->cb;
		pIn1n = t->nb;

		pIn2 = t->vc;
		pIn2Uv = t->uc;
		pIn2Cv = t->cc;
		pIn2n = t->nc;

		pOut1 = t->va;
		pOut1Uv = t->ua;
		pOut1Cv = t->ca;
		pOut1n = t->na;
	}
	else
	{
		//?
		return 0;
	}
	pOut2 = pOut1;
	pOut2Uv = pOut1Uv;
	pOut2Cv = pOut1Cv;
	pOut2n = pOut1n;
	float outP2Factor = 0.0f;
	c->ClipSegmentToFrustrum(&pIn1, &pOut1, outP2Factor);
	RecomputeUv(&pIn1Uv, &pOut1Uv, outP2Factor);
	RecomputeColor(&pIn1Cv, &pOut1Cv, outP2Factor);
	RecomputeNormal(&pIn1n, &pOut1n, outP2Factor);
	c->ClipSegmentToFrustrum(&pIn2, &pOut2, outP2Factor);
	RecomputeUv(&pIn2Uv, &pOut2Uv, outP2Factor);
	RecomputeColor(&pIn2Cv, &pOut2Cv, outP2Factor);
	RecomputeNormal(&pIn2n, &pOut2n, outP2Factor);

	if (m_TriangleQueueSize < m_maxTrianglesQueueSize)
	{
		Triangle* nt = &m_TrianglesQueue[m_TriangleQueueSize];
		Triangle::CopyTriangle(nt, t);
		nt->va->Copy(&pIn1);
		nt->ca->Copy(&pIn1Cv);	
		nt->ua->Copy(&pIn1Uv);
		nt->na->Copy(&pIn1n);
		nt->vb->Copy(&pIn2);
		nt->cb->Copy(&pIn2Cv);
		nt->ub->Copy(&pIn2Uv);
		nt->nb->Copy(&pIn2n);
		nt->vc->Copy(&pOut1);
		nt->cc->Copy(&pOut1Cv);
		nt->uc->Copy(&pOut1Uv);
		nt->nc->Copy(&pOut1n);
		RecomputeTriangleProj(c, nt);
		m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
		m_TriangleQueueSize++;
	}
	if (m_TriangleQueueSize < m_maxTrianglesQueueSize)
	{
		Triangle* nt = &m_TrianglesQueue[m_TriangleQueueSize];
		Triangle::CopyTriangle(nt, t);
		nt->va->Copy(&pIn2);
		nt->ca->Copy(&pIn2Cv);
		nt->ua->Copy(&pIn2Uv);
		nt->na->Copy(&pIn2n);
		nt->vb->Copy(&pOut2);
		nt->cb->Copy(&pOut2Cv);
		nt->ub->Copy(&pOut2Uv);
		nt->nb->Copy(&pOut2n);
		nt->vc->Copy(&pOut1);
		nt->cc->Copy(&pOut1Cv);
		nt->uc->Copy(&pOut1Uv);
		nt->nc->Copy(&pOut1n);
		RecomputeTriangleProj(c, nt);
		m_drawnTriangles += QueueTriangleInRasters(&m_TrianglesQueue[m_TriangleQueueSize], m_TriangleQueueSize);
		m_TriangleQueueSize++;
	}
	return 2;
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

void Engine::ComputeBoundingBoxes(const ZobMatrix4x4* modelMatrix, const ZobVector3* minBounding, const ZobVector3* maxBounding, Box* obb, Box* aabb) const
{
	ZobVector3 v0 = ZobVector3(minBounding->x, minBounding->y, minBounding->z);
	ZobVector3 v1 = ZobVector3(minBounding->x, maxBounding->y, minBounding->z);
	ZobVector3 v2 = ZobVector3(maxBounding->x, maxBounding->y, minBounding->z);
	ZobVector3 v3 = ZobVector3(maxBounding->x, minBounding->y, minBounding->z);
	ZobVector3 v4 = ZobVector3(minBounding->x, minBounding->y, maxBounding->z);
	ZobVector3 v5 = ZobVector3(minBounding->x, maxBounding->y, maxBounding->z);
	ZobVector3 v6 = ZobVector3(maxBounding->x, maxBounding->y, maxBounding->z);
	ZobVector3 v7 = ZobVector3(maxBounding->x, minBounding->y, maxBounding->z);

	obb->p0 = v0;
	obb->p1 = v1;
	obb->p2 = v2;
	obb->p3 = v3;
	obb->p4 = v4;
	obb->p5 = v5;
	obb->p6 = v6;
	obb->p7 = v7;
	modelMatrix->Mul(&obb->p0);
	modelMatrix->Mul(&obb->p1);
	modelMatrix->Mul(&obb->p2);
	modelMatrix->Mul(&obb->p3);
	modelMatrix->Mul(&obb->p4);
	modelMatrix->Mul(&obb->p5);
	modelMatrix->Mul(&obb->p6);
	modelMatrix->Mul(&obb->p7);

	ZobVector3 max;
	max.x = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(obb->p0.x, obb->p1.x), obb->p2.x), obb->p3.x), obb->p4.x), obb->p5.x), obb->p6.x), obb->p7.x);
	max.y = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(obb->p0.y, obb->p1.y), obb->p2.y), obb->p3.y), obb->p4.y), obb->p5.y), obb->p6.y), obb->p7.y);
	max.z = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(obb->p0.z, obb->p1.z), obb->p2.z), obb->p3.z), obb->p4.z), obb->p5.z), obb->p6.z), obb->p7.z);
	ZobVector3 min;
	min.x = fminf(fminf(fminf(fminf(fminf(fminf(fminf(obb->p0.x, obb->p1.x), obb->p2.x), obb->p3.x), obb->p4.x), obb->p5.x), obb->p6.x), obb->p7.x);
	min.y = fminf(fminf(fminf(fminf(fminf(fminf(fminf(obb->p0.y, obb->p1.y), obb->p2.y), obb->p3.y), obb->p4.y), obb->p5.y), obb->p6.y), obb->p7.y);
	min.z = fminf(fminf(fminf(fminf(fminf(fminf(fminf(obb->p0.z, obb->p1.z), obb->p2.z), obb->p3.z), obb->p4.z), obb->p5.z), obb->p6.z), obb->p7.z);

	ZobVector3 v = ZobVector3(
		(max.x - min.x) / 2.0f,
		(max.y - min.y) / 2.0f,
		(max.z - min.z) / 2.0f
	);
	v0 = ZobVector3(min.x, min.y, min.z);
	v1 = ZobVector3(min.x, max.y, min.z);
	v2 = ZobVector3(max.x, max.y, min.z);
	v3 = ZobVector3(max.x, min.y, min.z);
	v4 = ZobVector3(min.x, min.y, max.z);
	v5 = ZobVector3(min.x, max.y, max.z);
	v6 = ZobVector3(max.x, max.y, max.z);
	v7 = ZobVector3(max.x, min.y, max.z);

	aabb->p0 = v0;
	aabb->p1 = v1;
	aabb->p2 = v2;
	aabb->p3 = v3;
	aabb->p4 = v4;
	aabb->p5 = v5;
	aabb->p6 = v6;
	aabb->p7 = v7;
}

BoudingBox2D Engine::Get2DBoundingBox(const ZobObject* z) const
{
	bool bOk = false;
	float minX = 1000;
	float minY = 1000;
	float maxX = -1000;
	float maxY = -1000;
	if (z)
	{
		const Box* b = NULL;
		const std::vector<ZobBehavior*>* behaviors = z->GetBehaviors();
		if (behaviors->size() > 0)
		{
			for (int i = 0; i < behaviors->size(); i++)
			{
				const ZobBehavior* zb = behaviors->at(i);
				if (zb->GetBehaviorType() == ZobBehavior::eBehavior_mesh)
				{
					const ZobBehaviorMesh* zbm = (const ZobBehaviorMesh*)zb;
					const Mesh* m = zbm->GetMesh();
					b = m->GetOBB();
				}
			}
		}
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera();
		if (c && b)
		{
			ZobVector3 v;
			v = b->p0;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p1;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p2;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p3;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p4;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p5;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p6;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);

			v = b->p7;
			c->ProjectPointFromWorld(&v);
			minX = fminf(minX, v.x);
			maxX = fmaxf(maxX, v.x);
			minY = fminf(minY, v.y);
			maxY = fmaxf(maxY, v.y);
			bOk = true;
		}
	}
	BoudingBox2D b2d;
	if (bOk)
	{
		b2d._minX = minX;
		b2d._minY = minY;
		b2d._maxX = maxX;
 		b2d._maxY = maxY;
		b2d._minX = (b2d._minX + 1) * m_bufferData.width / 2.0f;
		b2d._maxX = (b2d._maxX + 1) * m_bufferData.height / 2.0f;
		b2d._minY = (b2d._minY + 1) * m_bufferData.width / 2.0f;
		b2d._maxY = (b2d._maxY + 1) * m_bufferData.height / 2.0f;
	}
	else
	{
		b2d._minX = 0;
		b2d._minY = 0;
		b2d._maxX = 0;
		b2d._maxY = 0;
	}
	return b2d;
}

bool Engine::IsInFrustrum(const Camera* c, const Box* aabb) const
{
	ZobVector3 minaabb, maxaabb;
	maxaabb.x = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(aabb->p0.x, aabb->p1.x), aabb->p2.x), aabb->p3.x), aabb->p4.x), aabb->p5.x), aabb->p6.x), aabb->p7.x);
	maxaabb.y = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(aabb->p0.y, aabb->p1.y), aabb->p2.y), aabb->p3.y), aabb->p4.y), aabb->p5.y), aabb->p6.y), aabb->p7.y);
	maxaabb.z = fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(fmaxf(aabb->p0.z, aabb->p1.z), aabb->p2.z), aabb->p3.z), aabb->p4.z), aabb->p5.z), aabb->p6.z), aabb->p7.z);
	ZobVector3 min;
	minaabb.x = fminf(fminf(fminf(fminf(fminf(fminf(fminf(aabb->p0.x, aabb->p1.x), aabb->p2.x), aabb->p3.x), aabb->p4.x), aabb->p5.x), aabb->p6.x), aabb->p7.x);
	minaabb.y = fminf(fminf(fminf(fminf(fminf(fminf(fminf(aabb->p0.y, aabb->p1.y), aabb->p2.y), aabb->p3.y), aabb->p4.y), aabb->p5.y), aabb->p6.y), aabb->p7.y);
	minaabb.z = fminf(fminf(fminf(fminf(fminf(fminf(fminf(aabb->p0.z, aabb->p1.z), aabb->p2.z), aabb->p3.z), aabb->p4.z), aabb->p5.z), aabb->p6.z), aabb->p7.z);
	return c->AABBIsInFrustrum(&minaabb, &maxaabb);
}

void Engine::UpdateEditorBitmapData()
{
	//memcpy(m_editorBuffer, m_buffer[m_currentBuffer], m_bufferData.size * 4);
}

void Engine::PrintRasterizersInfos()
{
	std::string o = "";
	ZobHUDManager* m = DirectZob::GetInstance()->GetHudManager();
	ZobVector3 c = ZobVector3(1, 0, 0);
	float y = 0.05f;
	for (int i = 0; i < m_nbRasterizers; i++)
	{
		m->Print(ZobHUDManager::eHudUnit_ratio, 0.5f, y, 1.5f, "MV Boli", &c, " R %i : %i", i, m_rasterizers[i]->GetNbTriangle());
		y += 0.02f;
	}
}

void Engine::EnablePerspectiveCorrection(bool enable)
{
	m_perspCorrection = enable;
}

void Engine::LoadFromNode(TiXmlElement* node)
{
	if (node)
	{
		m_varExposer->ReadNode(node);
	}
}

void Engine::SaveUnderNode(TiXmlElement* node)
{
	m_varExposer->SaveUnderNode(node);
}