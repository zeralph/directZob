#include "ZobFont.h"
#include "lodepng.h"
#include "BaseFont.h"
#include "../Types.h"
#include "../DirectZob.h"
#include "../Managers/MaterialManager.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace Core;

ZobFont::ZobFont(const std::string& file, int nbCharWidth, int nbCharHeight)
{
	Texture tex;
	tex.LoadFromFile(file);
	const float* data = tex.GetData();
	InitFont(data, tex.GetWidth(), tex.GetHeight(), nbCharWidth, nbCharHeight);
	//ZobFont(data, m_baseTexture.GetWidth(), m_baseTexture.GetHeight(), nbCharWidth, nbCharHeight);
}

ZobFont::ZobFont(const u8* data, int width, int height, int nbCharWidth, int nbCharHeight)
{
	float* fdata = (float*)malloc(sizeof(float) * width * height * 4);
	int k = 0;
	for(int i=0; i<width*height; i++)
	{
		float v = (data[i] == '1') ? 1.0f : 0.0f;
		fdata[k + 0] = v;
		fdata[k + 1] = v;
		fdata[k + 2] = v;
		fdata[k + 3] = v;
		k += 4;
	}
	InitFont(fdata, width, height, nbCharWidth, nbCharHeight);
	free(fdata);
}

ZobFont::ZobFont(const float* data, int width, int height, int nbCharWidth, int nbCharHeight)
{
	InitFont(data, width, height, nbCharWidth, nbCharHeight);
}

ZobFont::~ZobFont()
{

}

void ZobFont::InitFont(const float* data, int width, int height, int nbCharWidth, int nbCharHeight)
{
	m_color = ZobVector3(1, 1, 1);
	m_nbCharWidth = nbCharWidth;
	m_nbCharHeight = nbCharHeight;
	m_charMaterials = (const ZobMaterial**)malloc(sizeof(ZobMaterial*) * m_nbCharHeight * m_nbCharWidth);
	m_charWidth = width / m_nbCharWidth;
	m_charHeight = height / m_nbCharHeight;
	for (int c = 0; c < m_nbCharHeight * m_nbCharWidth; c++)
	{
		float* tdata = (float*)malloc(sizeof(float) * m_charHeight * m_charWidth * 4);
		uint ii = c % m_nbCharWidth * m_charWidth;
		uint jj = c / m_nbCharWidth * m_charHeight;
		int k = 0;
		for (int j = 0; j < m_charHeight; j++)
		{
			for (int i = 0; i < m_charWidth; i++)
			{
				int idx = ((jj + j) * width + (ii + i)) * 4;
				tdata[k + 0] = data[idx + 0];
				tdata[k + 1] = data[idx + 1];
				tdata[k + 2] = data[idx + 2];
				tdata[k + 3] = data[idx + 3];
				k += 4;
			}
		}
		std::string matName = "FONT";
		matName += '_';
		matName += std::to_string(c);
		Texture* tex = new Texture();
		tex->LoadFromData(matName, m_charWidth, m_charHeight, tdata);
		const ZobMaterial* zm = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(matName, &m_color, &m_color, tex);
		m_charMaterials[c] = zm;
	}
}

const ZobMaterial* ZobFont::GetChar(char c) const
{
	int i = (int)c;// -32;
	if (i > 0 && i < m_nbCharWidth * m_nbCharHeight)
	{
		const ZobMaterial* m = m_charMaterials[i];
		return m;
	}
	return NULL;
}