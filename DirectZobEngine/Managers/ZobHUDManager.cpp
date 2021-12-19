#include <thread>
#include <iostream>
#include <filesystem>
#include "ZobHUDManager.h"
#include "DirectZob.h"
#include "../SceneLoader.h"
#include <BaseFont.h>
#include <string>
#include "Embed/defaultFontData.h"
#include "Embed/defaultFontImage.h"
#undef None
#include "../../dependencies/optick/include/optick.h"
#define NB_HUD_TRIANGLES 2000
#define DEFAULT_FONT "Arial"

ZobHUDManager::ZobHUDManager()
{
	m_fonts.clear();
	m_started = false;
	m_renderOptions.zBuffered = false;
	m_renderOptions.bTransparency = false;
	m_renderOptions.cullMode = Triangle::RenderOptions::eCullMode_None;
	m_renderOptions.lightMode = Triangle::RenderOptions::eLightMode_none;
	m_trianglesBuffer = (Triangle*)malloc(sizeof(Triangle) * NB_HUD_TRIANGLES);
	m_vertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_uvs = (ZobVector2*)malloc(sizeof(ZobVector2) * NB_HUD_TRIANGLES * 3);
	m_colors = (ZobColor*)malloc(sizeof(ZobColor) * NB_HUD_TRIANGLES * 3);
	m_projectedVertices = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_normals = (ZobVector3*)malloc(sizeof(ZobVector3) * NB_HUD_TRIANGLES * 3);
	m_nbDrawnTriangles = 0;
	m_hudElements.clear();
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
		m_trianglesBuffer[i].zobObject = NULL;
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
	DeleteFonts();
}

//static Texture sTex;
static const ZobMaterial* sMat;
void ZobHUDManager::Init()
{
	std::string dir = ZobUtils::GetTempDirectory();

	std::string defaultFontDataFile = dir;
	defaultFontDataFile.append("defaultFont.xml");
	std::FILE* tmpf1 = fopen(defaultFontDataFile.c_str(), "w");
	//std::fwrite(defaultFontData, sizeof(int), defaultFontImage_length, tmpf1);
	std::fputws(defaultFontData, tmpf1);
	std::rewind(tmpf1);
	std::fclose(tmpf1);

	string defaultFontImageFile = dir;
	defaultFontImageFile.append("defaultFont.png");
	std::FILE* tmpf2 = fopen(defaultFontImageFile.c_str(), "wb");
	std::fwrite(defaultFontImage, sizeof(unsigned char), defaultFontImage_length, tmpf2);
	std::rewind(tmpf2);
	std::fclose(tmpf2);

	ZobFilePath zfpTexture;
	ZobFilePath zfpXml;

	zfpTexture = ZobFilePath(DEFAULT_FONT, dir, "defaultFont.png", true);
	zfpXml = ZobFilePath(DEFAULT_FONT, dir, "defaultFont.xml", true);
	m_fonts.push_back( new ZobFont(zfpTexture, zfpXml));

/*
	zfpTexture = ZobFilePath("mv_boli_regular_14", "_fonts", "mv_boli_regular_14.PNG", false);
	zfpXml = ZobFilePath("mv_boli_regular_14", "_fonts", "mv_boli_regular_14.xml", false);
	m_fonts.push_back( new ZobFont(zfpTexture, zfpXml));
		
	zfpTexture.Reset();
	zfpXml.Reset();
	zfpTexture = ZobFilePath("mv_boli_regular_32", "_fonts", "mv_boli_regular_32.PNG", false);
	zfpXml = ZobFilePath("mv_boli_regular_32", "_fonts", "mv_boli_regular_32.xml", false);
	m_fonts.push_back(new ZobFont(zfpTexture, zfpXml));

	zfpTexture.Reset();
	zfpXml.Reset();
	zfpTexture = ZobFilePath("leelawadee_ui_bold_32", "_fonts", "leelawadee_ui_bold_32.PNG", false);
	zfpXml = ZobFilePath("leelawadee_ui_bold_32", "_fonts", "leelawadee_ui_bold_32.xml", false);
	m_fonts.push_back(new ZobFont(zfpTexture, zfpXml));

	zfpTexture.Reset();
	zfpXml.Reset();
	zfpTexture = ZobFilePath("arial_regular_32", "_fonts", "arial_regular_32.PNG", false);
	zfpXml = ZobFilePath("arial_regular_32", "_fonts", "arial_regular_32.xml", false);
	m_fonts.push_back(new ZobFont(zfpTexture, zfpXml));

	zfpTexture.Reset();
	zfpXml.Reset();
	zfpTexture = ZobFilePath("vcr_osd_mono_regular_14", "_fonts", "vcr_osd_mono_regular_14.PNG", false);
	zfpXml = ZobFilePath("vcr_osd_mono_regular_14", "_fonts", "vcr_osd_mono_regular_14.xml", false);
	m_fonts.push_back(new ZobFont(zfpTexture, zfpXml));
*/
}

void ZobHUDManager::Stop()
{
	m_started = false;
	DeleteFonts();
	m_hudElements.clear();
	m_nbDrawnTriangles = 0;
}

void ZobHUDManager::Start()
{
	Init();
	m_started = true;
}

void ZobHUDManager::PreUpdate(float dt)
{
}

void ZobHUDManager::UpdateBehavior(float dt) 
{
	
}

void ZobHUDManager::DeleteFonts()
{
	for (std::vector<const ZobFont*>::const_iterator iter = m_fonts.begin(); iter != m_fonts.end(); iter++)
	{
		delete (*iter);
	}
	m_fonts.clear();
}

void ZobHUDManager::UpdateObjects(const Camera* camera, Engine* engine, float dt)
{
	OPTICK_EVENT();
	for (std::vector<HUDElement>::const_iterator iter = m_hudElements.begin(); iter != m_hudElements.end(); iter++)
	{
		HUDElement e = *iter;
		float xMin;
		float yMin;
		float xMax;
		float yMax;
		xMin = e.x;
		yMin = e.y;
		xMax = xMin + e.w;
		yMax = yMin + e.h;
		CreateQuad(xMin, yMin, xMax, yMax, &e);
	}
}

void ZobHUDManager::QueueForDrawing(const Camera* camera, Engine* engine)
{
	OPTICK_EVENT();
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
	float uv_a_x = -1;
	float uv_a_y = 0;

	float uv_b_x = 0;
	float uv_b_y = 1;

	float uv_c_x = -1;
	float uv_c_y = 1;

	float uv_d_x = 0;
	float uv_d_y = 0;
	if (elem->glyphe != NULL)
	{
		uv_d_x = elem->glyphe->uv_max_x;
		uv_d_y = -elem->glyphe->uv_max_y;

		uv_b_x = elem->glyphe->uv_max_x;
		uv_b_y = -elem->glyphe->uv_min_y;

		uv_a_x = elem->glyphe->uv_min_x;
		uv_a_y = -elem->glyphe->uv_max_y;

		uv_c_x = elem->glyphe->uv_min_x;
		uv_c_y = -elem->glyphe->uv_min_y;
	}
	float z = DirectZob::GetInstance()->GetEngine()->GetBufferData()->zNear + 1.0f;
	Triangle* t1 = &m_trianglesBuffer[m_nbDrawnTriangles];
	t1->pc->x = xMin;
	t1->pc->y = yMin;
	t1->pc->z = z;
	t1->uc->x = uv_c_x;
	t1->uc->y = uv_c_y;
	t1->pb->x = xMax;
	t1->pb->y = yMin;
	t1->pb->z = z;
	t1->ub->x = uv_b_x;
	t1->ub->y = uv_b_y;
	t1->pa->x = xMin;
	t1->pa->y = yMax;
	t1->pa->z = z;
	t1->ua->x = uv_a_x;
	t1->ua->y = uv_a_y;
	t1->material = elem->mat;
	t1->ca->Copy(&elem->color);
	t1->cb->Copy(&elem->color);
	t1->cc->Copy(&elem->color);
	//t1->ca->Copy(&sca);
	//t1->cb->Copy(&scb);
	//t1->cc->Copy(&scc);
	t1->ComputeArea();
	//t1->area = -t1->area;
	m_nbDrawnTriangles++;

	Triangle* t2 = &m_trianglesBuffer[m_nbDrawnTriangles];
	t2->pc->x = xMax;
	t2->pc->y = yMin;
	t2->pc->z = z;
	t2->uc->x = uv_b_x;
	t2->uc->y = uv_b_y;
	t2->pb->x = xMax;
	t2->pb->y = yMax;
	t2->pb->z = z;
	t2->ub->x = uv_d_x;
	t2->ub->y = uv_d_y;
	t2->pa->x = xMin;
	t2->pa->y = yMax;
	t2->pa->z = z;
	t2->ua->x = uv_a_x;
	t2->ua->y = uv_a_y;
	t2->material = elem->mat;
	t2->ca->Copy(&elem->color);
	t2->cb->Copy(&elem->color);
	t2->cc->Copy(&elem->color);
	//t1->ca->Copy(&scc);
	//t2->cb->Copy(&scb);
	//t2->cc->Copy(&scd);
	t2->ComputeArea();
	//t2->area = -t2->area;
	m_nbDrawnTriangles++;
	return true;
}

void ZobHUDManager::Print(eHudUnit u, float x, float y, float fontSize, const char* fontName, const ZobVector3* color, const char* fmt, ...)
{
	if (m_started && DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()) //if (m_engine->ShowText() && m_data != NULL)
	{
		const ZobFont* f = GetFont(fontName);
		if (f)
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
			PrintInternal(u, x, y, fontSize, f, color, buf);
		}
	}
}


void ZobHUDManager::PrintInternal(eHudUnit u, float x, float y, float fontSize, const ZobFont* font, const ZobVector3* color, std::string s)
{
	if (font && font->IsLoaded())
	{
		float screenW = DirectZob::GetInstance()->GetEngine()->GetBufferData()->width;
		float screenH = DirectZob::GetInstance()->GetEngine()->GetBufferData()->height;
		int horiz = 0;
		int vert = 0;
		int size = s.size();
		if (u == eHudUnit_ratio)
		{
			x *= screenW;
			y *= screenH;
		}
		for (size_t i = 0; i < size; i++)
		{
			char c = s[i];
			if (c != '\0')
			{
				if (c != '\n')
				{
					if (c == ' ')
					{
						int u = 0;
						u++;
					}
					HUDElement e;
					e.scaleX = fontSize;
					e.scaleY = fontSize;
					const ZobFont::FontGlyphe* fg = font->GetChar(c);
					e.color = color;
					e.x = x + horiz + fg->offsetX * fontSize;
					e.y = y + vert + fg->offsetY * fontSize;
					e.w = fg->w * fontSize;
					e.h = fg->h * fontSize;
					e.unit = u;
					e.c = c;
					e.glyphe = font->GetChar(c);
					assert(e.glyphe);
					e.mat = e.glyphe->mat;
					m_hudElements.push_back(e);
					horiz+= fg->width * fontSize;
				}
				else
				{
					horiz = 0;
					vert+= font->GetHeight() * fontSize;
				}
			}
		}
	}
}

const ZobFont* ZobHUDManager::GetFont(const std::string& fontName) const
{
	for (std::vector<const ZobFont*>::const_iterator iter = m_fonts.begin(); iter != m_fonts.end(); iter++)
	{
		if ((*iter)->GetName() == fontName)
		{
			return (*iter);
		}
	}
	if (m_fonts.size() > 0)
	{
		return m_fonts[0];
	}
	return NULL;
}