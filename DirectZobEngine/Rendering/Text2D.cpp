#include "text2D.h"
#include "lodepng.h"
#include "BaseFont.h"
#include "../Types.h"
#include <stdio.h>
#include <string>
#include <iostream>

static char buffer[MAX_PATH];

Text2D::Text2D(Engine* engine, Events* events)
{
	
	m_data = BaseFont.pixel_data;
	m_texWidth = BaseFont.width;
	m_texHeight = BaseFont.height;

	m_events = events;
	m_engine = engine;
	m_nbCharWidth = BaseFont.nbCharWidth;
	m_nbCharHeight = BaseFont.nbCharHeight;
	m_charWidth = m_texWidth / BaseFont.nbCharWidth;
	m_charHeight = m_texHeight / BaseFont.nbCharHeight;
	m_matrix.Identity();
}

Text2D::~Text2D()
{
}

void Text2D::GetChar(char c, Texture& tex) const
{
}

void Text2D::Print(uint x, uint y, const std::string* text, uint color)
{
	if (m_engine->ShowText() && m_data != NULL)
	{
		size_t l = text->length();
		for (size_t i = 0; i < l; i++)
		{
			int xx = x + i * m_charWidth;
			PrintChar(xx, y, text->at(i), color);
		}
	}
}

void Text2D::Print(uint x, uint y, uint color, const char* fmt, ...)
{
	if (m_engine->ShowText() && m_data != NULL)
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
		for (size_t i = 0; i < size+1; i++)
		{
			int xx = x + i * m_charWidth;
			PrintChar(xx, y, buf[i], color);
		}
	}
}

void Text2D::PrintChar(uint x, uint y, const char c, uint color)
{
	uint tw = m_texWidth;
	uint th = m_texHeight;
	uint ew = m_engine->GetBufferData()->width;
	uint eh = m_engine->GetBufferData()->height;
	uint bSize = m_engine->GetBufferData()->size;
	uint pc = (int)c;
	uint ii = pc % m_nbCharWidth * m_charWidth;
	uint jj = pc / m_nbCharWidth * m_charHeight;
	uint k = 0;
	for (int j = 0; j < m_charHeight; j++)
	{
		for (int i = 0; i < m_charWidth; i++)
		{
			char c = m_data[(jj+j)*tw + (ii+i) ];
			k = (y + j) * ew + (x + i);
			if (k<bSize && c == '1')
			{
				m_engine->GetBufferData()->buffer[k] = color;
				m_engine->GetBufferData()->zBuffer[k] = 0;
			}
		}
	}

	
		
}