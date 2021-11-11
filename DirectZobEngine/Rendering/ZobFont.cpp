#include "ZobFont.h"
#include "lodepng.h"
#include "BaseFont.h"
#include "../Types.h"
#include "../DirectZob.h"
#include "../Managers/MaterialManager.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include "tinyxml.h"

ZobFont::ZobFont(const std::string& textureFile, const std::string& xmlFile)
{
	TiXmlDocument doc("Font");
	doc.ClearError();
	doc.LoadFile(xmlFile.c_str());
	if (doc.Error())
	{
		DirectZob::LogError("[ZobFont] Error loading %s : %s", xmlFile.c_str(), doc.ErrorDesc());
	}
	else
	{
		TiXmlElement* font = doc.FirstChildElement("Font");
		if (font)
		{
			m_name = font->Attribute("family");
			std::vector<std::string> v;
			std::string s;
			std::string::size_type sz;
			m_height = std::stoi(font->Attribute("height"), &sz);
			for (TiXmlElement* e = font->FirstChildElement("Char"); e != NULL; e = e->NextSiblingElement("Char"))
			{
				FontGlyphe fg;
				fg.c = e->Attribute("code")[0];
				fg.charcode = (int)fg.c;
				fg.width = std::stoi(e->Attribute("width"), &sz);
				s = e->Attribute("offset");
				SplitEntry(&s, &v, ' ');
				fg.offsetX = std::stoi(v[0], &sz);
				fg.offsetY = std::stoi(v[1], &sz);
				s = e->Attribute("rect");
				SplitEntry(&s, &v, ' ');
				fg.x = std::stoi(v[0], &sz);
				fg.y = std::stoi(v[1], &sz);
				fg.w = std::stoi(v[2], &sz);
				fg.h = std::stoi(v[3], &sz);
				m_glyphes.push_back(fg);
			}
			m_color = ZobVector3(1, 1, 1);
			std::string matName = "Font_";
			matName.append(m_name);
			m_charMaterial = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(matName, &m_color, &m_color, &m_color, 0, 1, textureFile);
			float texW = m_charMaterial->GetDiffuseTexture()->GetWidth();
			float texH = m_charMaterial->GetDiffuseTexture()->GetHeight();
			for (std::vector<FontGlyphe>::iterator iter = m_glyphes.begin(); iter != m_glyphes.end(); iter++)
			{
				FontGlyphe* fg = &(*iter);
				fg->mat = m_charMaterial;
				fg->uv_min_x = (fg->x) / texW;
				fg->uv_min_y = (fg->y) / texH;
				fg->uv_max_x = (fg->x + fg->w) / texW;
				fg->uv_max_y = (fg->y + fg->h) / texH;
				assert(fg->uv_min_x <= fg->uv_max_x);
				assert(fg->uv_min_y <= fg->uv_max_y);
				assert(fg->uv_max_x <= texW);
				assert(fg->uv_max_y <= texH);
			}
		}
	}
	DirectZob::LogInfo("Font %s loaded", m_name.c_str());
}

ZobFont::~ZobFont()
{
	m_glyphes.clear();
}

const ZobFont::FontGlyphe* ZobFont::GetChar(char c) const
{
	for (std::vector<FontGlyphe>::const_iterator iter = m_glyphes.begin(); iter != m_glyphes.end(); iter++)
	{
		const FontGlyphe* fg = &(*iter);
		if (fg->c == c)
		{
			return fg;
		}
	}
	return NULL;
}


void ZobFont::SplitEntry(const std::string* s, std::vector<std::string>* v, const char delim) const
{
	v->clear();
	std::size_t current, previous = 0;
	current = s->find(delim);
	while (current != std::string::npos) {
		std::string sub = s->substr(previous, current - previous);
		if (sub.length() > 0 && sub != " ")
		{
			v->push_back(sub);
		}
		previous = current + 1;
		current = s->find(delim, previous);
	}
	std::string sub = s->substr(previous, current - previous);
	if (sub.length() > 0 && sub != " ")
	{
		v->push_back(sub);
	}
}