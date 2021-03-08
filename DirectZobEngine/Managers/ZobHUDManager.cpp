#include <thread>
#include "ZobHUDManager.h"
#include "DirectZob.h"
#include "../SceneLoader.h"
#include <BaseFont.h>
#define NB_HUD_TRIANGLES 2000

ZobHUDManager::ZobHUDManager()
{
	m_font = NULL;
	m_started = false;
	m_renderOptions.zBuffered = false;
	m_renderOptions.bTransparency = false;
	m_renderOptions.cullMode = eCullMode_None;
	m_renderOptions.lightMode = DirectZobType::RenderOptions::eLightMode_none;
	m_trianglesBuffer = (Triangle*)malloc(sizeof(Triangle) * NB_HUD_TRIANGLES);
	m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * NB_HUD_TRIANGLES * 3);
	m_colors = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_normals = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_nbDrawnTriangles = 0;
	int vi = 0;
	for (int i = 0; i < NB_HUD_TRIANGLES; i++)
	{
		m_trianglesBuffer[i].verticeAIndex = vi;
		m_trianglesBuffer[i].verticeBIndex = vi + 1;
		m_trianglesBuffer[i].verticeCIndex = vi + 2;
		m_trianglesBuffer[i].va = &m_vertices[vi];
		m_trianglesBuffer[i].vb = &m_vertices[vi + 1];
		m_trianglesBuffer[i].vc = &m_vertices[vi + 2];
		m_trianglesBuffer[i].pa = &m_projectedVertices[vi];
		m_trianglesBuffer[i].pb = &m_projectedVertices[vi + 1];
		m_trianglesBuffer[i].pc = &m_projectedVertices[vi + 2];
		m_trianglesBuffer[i].na = &m_normals[vi];
		m_trianglesBuffer[i].nb = &m_normals[vi + 1];
		m_trianglesBuffer[i].nc = &m_normals[vi + 2];
		m_trianglesBuffer[i].ua = &m_uvs[vi];
		m_trianglesBuffer[i].ub = &m_uvs[vi + 1];
		m_trianglesBuffer[i].uc = &m_uvs[vi + 2];
		m_trianglesBuffer[i].ca = &m_colors[vi];
		m_trianglesBuffer[i].cb = &m_colors[vi + 1];
		m_trianglesBuffer[i].cc = &m_colors[vi + 2];
		m_trianglesBuffer[i].n = &m_normals[i];
		m_trianglesBuffer[i].options = &m_renderOptions;
		m_trianglesBuffer[i].material = NULL;
		vi += 3;
	}
}

ZobHUDManager::~ZobHUDManager()
{
	free(m_trianglesBuffer);
	free(m_vertices);
	free(m_uvs);
	free(m_projectedVertices);
	free(m_normals);
	delete m_font;
}

//static Texture sTex;
static const ZobMaterial* sMat;
void ZobHUDManager::Init()
{
	std::string p = SceneLoader::GetResourcePath();
	if (p.length())
	{
		p.append("font2.png");
		m_font = new ZobFont(p, 32, 8);
		//m_font = new ZobFont("D:\\Git\\directZob\\resources\\font3.png", 16, 14);
	}
	else
	{
		const u8* data = BaseFont.pixel_data;
		m_font = new ZobFont(data, BaseFont.width, BaseFont.height, BaseFont.nbCharWidth, BaseFont.nbCharHeight);
	}

}

void ZobHUDManager::Stop()
{
	m_started = false;
	m_hudElements.clear();
	m_nbDrawnTriangles = 0;
}

void ZobHUDManager::Start()
{
	Init();
	m_started = true;
}

void ZobHUDManager::PreUpdate()
{
}

void ZobHUDManager::UpdateBehavior(float dt)
{
	
}

void ZobHUDManager::UpdateObjects(const Camera* camera, Core::Engine* engine, float dt)
{
	for (std::vector<HUDElement>::const_iterator iter = m_hudElements.begin(); iter != m_hudElements.end(); iter++)
	{
		HUDElement e = *iter;
		float w = engine->GetBufferData()->width;
		float h = engine->GetBufferData()->height;
		//CreateQuad(e.x * w, e.y * h, e.w * w, e.h * h, e.mat);
		float xMin = e.x * w;
		float yMin = e.y * h;
		float xMax = xMin + e.w * w;
		float yMax = yMin + e.w * h;

		//xMin = e.x * w;
		//yMin = e.y * h;
		//xMax = xMin + 8;
		//yMax = yMin + 8;


		CreateQuad(xMin, yMin, xMax, yMax, &e);
	}
}

void ZobHUDManager::QueueForDrawing(const Camera* camera, Core::Engine* engine)
{
	for (int i = 0; i < m_nbDrawnTriangles; i++)
	{
		engine->QueueProjectedTriangle(camera, &m_trianglesBuffer[i]);
	}
	m_hudElements.clear();
	m_nbDrawnTriangles = 0;
}

static ZobVector3 sca = ZobVector3(1, 0, 0);
static ZobVector3 scb = ZobVector3(0, 1, 0);
static ZobVector3 scc = ZobVector3(0, 0, 1);
static ZobVector3 scd = ZobVector3(1, 0, 0);

bool ZobHUDManager::CreateQuad(float xMin, float yMin, float xMax, float yMax, HUDElement* elem)
{
	float z = DirectZob::GetInstance()->GetEngine()->GetBufferData()->zNear + 1.0f;
	Triangle* t1 = &m_trianglesBuffer[m_nbDrawnTriangles];
	t1->pa->x = xMin;
	t1->pa->y = yMin;
	t1->pa->z = z;
	t1->ua->x = -1;
	t1->ua->y = 1;
	t1->pb->x = xMax;
	t1->pb->y = yMin;
	t1->pb->z = z;
	t1->ub->x = -0;
	t1->ub->y = 1;
	t1->pc->x = xMin;
	t1->pc->y = yMax;
	t1->pc->z = z;
	t1->uc->x = -1;
	t1->uc->y = 0;
	t1->material = elem->mat;
	t1->ca->Copy(&elem->color);
	t1->cb->Copy(&elem->color);
	t1->cc->Copy(&elem->color);
	//t1->ca->Copy(&sca);
	//t1->cb->Copy(&scb);
	//t1->cc->Copy(&scc);
	t1->ComputeArea();
	m_nbDrawnTriangles++;

	Triangle* t2 = &m_trianglesBuffer[m_nbDrawnTriangles];
	t2->pa->x = xMax;
	t2->pa->y = yMin;
	t2->pa->z = z;
	t2->ua->x = 0;
	t2->ua->y = 1;
	t2->pb->x = xMax;
	t2->pb->y = yMax;
	t2->pb->z = z;
	t2->ub->x = 0;
	t2->ub->y = 0;
	t2->pc->x = xMin;
	t2->pc->y = yMax;
	t2->pc->z = z;
	t2->uc->x = -1;
	t2->uc->y = 0;
	t2->material = elem->mat;
	t2->ca->Copy(&elem->color);
	t2->cb->Copy(&elem->color);
	t2->cc->Copy(&elem->color);
	//t1->ca->Copy(&scc);
	//t2->cb->Copy(&scb);
	//t2->cc->Copy(&scd);
	t2->ComputeArea();
	m_nbDrawnTriangles++;
	return true;
}

void ZobHUDManager::Print(float x, float y, float w, float h, const ZobVector3* color, const char* fmt, ...)
{
	if (m_font && m_started) //if (m_engine->ShowText() && m_data != NULL)
	{
		//size_t size = strlen(fmt) + 1;
		va_list vl;
		va_start(vl, fmt);
		int size = _vscprintf(fmt, vl);
		std::string buf;
		buf.reserve(size + 1);
		buf.resize(size);
		_vsnprintf((char*)buf.data(), size, fmt, vl);
		va_end(vl);
		PrintInternal(x, y, w, h, color, buf);
	}
}

void ZobHUDManager::Print(float x, float y, float w, float h, const char* fmt, ...)
{
	if (m_font && m_started) //if (m_engine->ShowText() && m_data != NULL)
	{
		size_t size = strlen(fmt) + 1;
		va_list vl;
		va_start(vl, fmt);
		//int size = _vscprintf(fmt, vl);
		std::string buf;
		buf.reserve(size + 1);
		buf.resize(size);
		_vsnprintf((char*)buf.data(), size, fmt, vl);
		va_end(vl);
		ZobVector3 color = ZobVector3(0, 0, 0);
		PrintInternal(x, y, w, h, &color, buf);
	}
}

void ZobHUDManager::PrintInternal(float x, float y, float w, float h, const ZobVector3* color, std::string s)
{
	if (m_font)
	{
		int horiz = 0;
		int vert = 0;
		int size = s.size();
		for (size_t i = 0; i < size; i++)
		{
			char c = s[i];
			if (c != '\0')
			{
				if (c != '\n')
				{
					HUDElement e;
					e.color = color;
					e.x = x + w * horiz;
					e.y = y + h * vert;
					e.w = w;
					e.h = h;
					e.mat = m_font->GetChar(c);
					m_hudElements.push_back(e);
					horiz++;
				}
				else
				{
					horiz = 0;
					vert++;
				}
			}
		}
	}
}