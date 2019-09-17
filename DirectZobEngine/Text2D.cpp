#include "text2D.h"

using namespace Core;

Text2D::Text2D(Engine* engine, const Material* tex, int nbCharWidth, int nbCharHeight, Events* events)
{
	m_events = events;
	m_engine = engine;
	m_texture = tex;
	m_nbCharWidth = nbCharWidth;
	m_nbCharHeight = nbCharHeight;
	m_charWidth = (tex?tex->GetWidth():0) / nbCharWidth;
	m_charHeight = (tex?tex->GetHeight():0) / nbCharHeight;
	m_matrix.Identity();
}

Text2D::~Text2D()
{
}

void Text2D::Print(uint x, uint y, uint size, const std::string* text, uint color)
{
	if (m_texture != NULL)
	{
		size_t l = text->length();
		for (size_t i = 0; i < l; i++)
		{
			int xx = x + i * m_charWidth;
			PrintChar(xx, y, size, text->at(i), color);
		}
	}
}

void Text2D::PrintChar(uint x, uint y, uint size, const char c, uint color)
{
	uint tw = m_texture->GetWidth();
	uint th = m_texture->GetHeight();
	uint ew = m_engine->GetBufferData()->width;
	uint pc = (int)c;
	uint ii = pc % m_nbCharWidth * m_charWidth;
	uint jj = pc / m_nbCharWidth * m_charHeight;

	int k = 0;
	for (int j = 0; j < m_charHeight; j++)
	{
		for (int i = 0; i < m_charWidth; i++)
		{
			uint c = m_texture->GetData()[(jj+j)*tw*4 + (ii+i)*4 ];
			if (c == 0)
			{
				c = color;
				k = (y + j) * ew + (x + i);
				m_engine->GetBufferData()->buffer[k] = c;
			}
			else
			{
				//c = 0x00000000;
			}

		}
	}

	
		
}