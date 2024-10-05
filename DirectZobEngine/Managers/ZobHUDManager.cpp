#include <thread>
#include <iostream>
#include <filesystem>
#include "ZobHUDManager.h"
#include "../DirectZob.h"
#include "../SceneLoader.h"
#include "../BaseFont.h"
#include <string>
#include "../Embed/defaultFontData.h"
#include "../Embed/defaultFontImage.h"
#include <gainput/gainput.h>
#include "ZobInputManager.h"
#include "../Components/HudComponents/ZobComponentText.h"
#include "../ZobObjects/ZobObject.h"
#include "../ZobObjects/Camera.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/Light.h"
#include "../ZobMaterial.h"
#include "CameraManager.h"
#undef None
#include "../../dependencies/optick/include/optick.h"
#define NB_HUD_TRIANGLES 2000
#define DEFAULT_FONT "Arial"

char ZobHUDManager::m_buffer[HUD_TEXT_BUFFER_SIZE];

ZobHUDManager::ZobHUDManager()
{
	m_fonts.clear();
	m_started = false;
	m_menuItems.resize(0);
	m_renderOptions.zBuffer = Triangle::RenderOptions::buffered;
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
	m_menuCurrentIndex = 0;
	m_menuItems.resize(0);
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

	//zfpTexture = ZobFilePath(DEFAULT_FONT, dir, "defaultFont.png", true);
	//zfpXml = ZobFilePath(DEFAULT_FONT, dir, "defaultFont.xml", true);
	//CreateOrGetZobFont(zfpXml, zfpTexture);

}

void ZobHUDManager::AddToMenu(ZobComponentText* zct)
{
	m_menuItems.push_back(zct);
}

void ZobHUDManager::RemoveFromMenu(ZobComponentText* zct)
{
	std::vector<ZobComponentText*>::iterator iter = m_menuItems.begin();
	while (iter != m_menuItems.end())
	{
		if ((*iter) == zct)
		{
			m_menuItems.erase(iter);
			return;
		}
	}
}

const ZobFont* ZobHUDManager::CreateOrGetZobFont(ZobFilePath* zfpXml, ZobFilePath* zfpTexture)
{
	std::vector<const ZobFont*>::const_iterator iter = m_fonts.begin();
	while (iter != m_fonts.end())
	{
		std::string s = (*iter)->GetName();
		if (s == zfpXml->GetName())
		{
			return (*iter);
		}
		iter++;
	}
	ZobFont* f = new ZobFont(zfpXml, zfpTexture);
	m_fonts.push_back(f);
	return f;
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
	//Init();
	m_started = true;
}

void ZobHUDManager::UpdateNavigation(float dt)
{
	if (m_started && m_menuItems.size() > 0)
	{
		const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
		if (inputMap->GetBoolIsNew(ZobInputManager::Start))
		{
			m_menuItems[m_menuCurrentIndex]->DoAction();
		}
		m_menuItems[m_menuCurrentIndex]->SetSelected(false);
		if (inputMap->GetBoolIsNew(ZobInputManager::MenuUp))
		{
			m_menuCurrentIndex = max(m_menuCurrentIndex - 1, 0);
		}
		if (inputMap->GetBoolIsNew(ZobInputManager::MenuDown))
		{
			m_menuCurrentIndex = min(m_menuCurrentIndex + 1, (int)m_menuItems.size()-1);
		}
		m_menuItems[m_menuCurrentIndex]->SetSelected(true);
	}
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
	struct {
		bool operator()(HUDElement &a, HUDElement &b) const { return a.z < b.z; }
	} customLess;
	std::sort(m_hudElements.begin(), m_hudElements.end(), customLess);
	int idx = 0;
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
		CreateQuad(e.zo, xMin, yMin, xMax, yMax, idx, &e);
		idx++;
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

bool ZobHUDManager::CreateQuad(ZobObject* zobObj, float xMin, float yMin, float xMax, float yMax, int idx, HUDElement* elem)
{
	float uv_a_x = 0;
	float uv_a_y = 0;

	float uv_b_x = 1;
	float uv_b_y = 1;

	float uv_c_x = 0;
	float uv_c_y = 1;

	float uv_d_x = 1;
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
	float z = DirectZob::GetInstance()->GetEngine()->GetBufferData()->zNear + 0.00001f * (idx+1.0f);
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
	t1->zobObject = zobObj;
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
	t2->zobObject = zobObj;
	//t1->ca->Copy(&scc);
	//t2->cb->Copy(&scb);
	//t2->cc->Copy(&scd);
	t2->ComputeArea();
	//t2->area = -t2->area;
	m_nbDrawnTriangles++;
	return true;
}

void ZobHUDManager::Print(ZobObject* zobObj, eHudUnit u, float x, float y, float z, float fontSize, const ZobFont* font, const ZobColor* color, const char* fmt, ...)
{
	if (m_started && DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera()) //if (m_engine->ShowText() && m_data != NULL)
	{
		if (!font && m_fonts.size() > 0)
		{
			font = m_fonts[0];
			fontSize = 1;
			//u = eHudUnit::eHudUnit_pixels;
		}
		if (font && fmt)
		{
			//size_t size = strlen(fmt) + 1;
			va_list argList;
			va_start(argList, fmt);
			size_t charsNeeded = vsnprintf(NULL, 0, fmt, argList);
        	va_end(argList);
        	va_start(argList, fmt);
        	vsnprintf(m_buffer, charsNeeded+1, fmt, argList);
        	va_end(argList);
			PrintInternal(zobObj, u, x, y, z, fontSize, font, color, m_buffer, charsNeeded);
		}
	}
}

void  ZobHUDManager::Print(ZobObject* zobObj, eHudUnit u, float x, float y, float w, float h, float z, const ZobMaterial* mat)
{
	float screenW = DirectZob::GetInstance()->GetEngine()->GetBufferData()->width;
	float screenH = DirectZob::GetInstance()->GetEngine()->GetBufferData()->height;
	int horiz = 0;
	int vert = 0;
	if (u == eHudUnit_ratio)
	{
		x *= screenW;
		y *= screenH;
		w *= screenW;
		h *= screenH;
	}
	HUDElement e;
	e.x = x;
	e.y = y;
	e.w = w;
	e.h = h;
	e.z = z;
	e.zo = zobObj;
	e.mat = mat;
	e.glyphe = NULL;
	e.color = ZobColor::White;
	m_hudElements.push_back(e);
}

void ZobHUDManager::PrintInternal(ZobObject* zobObj, eHudUnit u, float x, float y, float z, float fontSize, const ZobFont* font, const ZobColor* color, const char* buf, size_t size)
{
	if (font && font->IsLoaded())
	{
		float screenW = DirectZob::GetInstance()->GetEngine()->GetBufferData()->width;
		float screenH = DirectZob::GetInstance()->GetEngine()->GetBufferData()->height;
		int horiz = 0;
		int vert = 0;
		if (u == eHudUnit_ratio)
		{
			x *= screenW;
			y *= screenH;
			fontSize = (screenH * fontSize) / 700;;
		}
		for (size_t i = 0; i < size; i++)
		{
			char c = buf[i];
			if (c != '\0')
			{
				if (c != '\n')
				{
					if (c != '\r')
					{
						HUDElement e;
						e.scaleX = fontSize;
						e.scaleY = fontSize;
						e.z = z;
						e.zo = zobObj;  
						const ZobFont::FontGlyphe* fg = font->GetChar(c);
						if (fg)
						{
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
					}
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
	return NULL;
}