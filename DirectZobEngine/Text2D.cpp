#include "text2D.h"
#include "lodepng.h"
using namespace Core;

Text2D::Text2D(Engine* engine, std::string& fontImageFile, int nbCharWidth, int nbCharHeight, Events* events)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, fontImageFile.c_str());
	if (error)
	{
		m_data = NULL;
		m_texWidth = 0;
		m_texHeight = 0;
	}
	else
	{
		m_texWidth = width;
		m_texHeight = height;
		m_data = (float*)malloc(sizeof(float) * 4 * image.size());
		for (int i = 0; i < image.size(); i += 4)
		{
			float r = (float)image[i] / 255.0f;
			float g = (float)image[i + 1] / 255.0f;
			float b = (float)image[i + 2] / 255.0f;
			float a = (float)image[i + 3] / 255.0f;
			m_data[i] = r;
			m_data[i + 1] = g;
			m_data[i + 2] = b;
			m_data[i + 3] = a;
		}
	}
	image.clear();
	m_events = events;
	m_engine = engine;
	m_nbCharWidth = nbCharWidth;
	m_nbCharHeight = nbCharHeight;
	m_charWidth = m_texWidth / nbCharWidth;
	m_charHeight = m_texHeight / nbCharHeight;
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
			uint c = m_data[(jj+j)*tw*4 + (ii+i)*4 ];
			if (c == 0)
			{
				c = color;
				k = (y + j) * ew + (x + i);
				m_engine->GetBufferData()->buffer[k] = c;
			}
		}
	}

	
		
}