#include "text2D.h"
#include "lodepng.h"
#include "BaseFont.h"
#include "../Types.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace Core;

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

void Text2D::Print(uint x, uint y, uint size, const std::string* text, uint color)
{
	if (m_data != NULL)
	{
		size_t l = text->length();
		for (size_t i = 0; i < l; i++)
		{
			int xx = x + i * m_charWidth;
			PrintChar(xx, y, size, text->at(i), color);
		}
	}
}

void Text2D::Print(uint x, uint y, uint size, uint color, const char* fmt, ...)
{
	if (m_data != NULL)
	{
		va_list vl;
		va_start(vl, fmt);
		int size = _vscprintf(fmt, vl);
		std::string buf;
		buf.reserve(size + 1);
		buf.resize(size);
		vsnprintf_s((char*)buf.data(), size, _TRUNCATE, fmt, vl);
		va_end(vl);
		for (size_t i = 0; i < size; i++)
		{
			int xx = x + i * m_charWidth;
			PrintChar(xx, y, size, buf[i], color);
		}
	}
}

void Text2D::PrintChar(uint x, uint y, uint size, const char c, uint color)
{
	uint tw = m_texWidth;
	uint th = m_texHeight;
	uint ew = m_engine->GetBufferData()->width;
	uint pc = (int)c;
	uint ii = pc % m_nbCharWidth * m_charWidth;
	uint jj = pc / m_nbCharWidth * m_charHeight;

	int k = 0;
	for (int j = 0; j < m_charHeight; j++)
	{
		for (int i = 0; i < m_charWidth; i++)
		{
			char c = m_data[(jj+j)*tw + (ii+i) ];
			if (c == '1')
			{
				k = (y + j) * ew + (x + i);
				m_engine->GetBufferData()->buffer[k] = color;
			}
		}
	}

	
		
}