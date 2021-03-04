#pragma once
#include "../Types.h"
#include "../ZobMaterial.h"
#include "ZobMatrix2x2.h"
#include "Engine.h"
#include "../Events.h"
#include "../ZobMaterial.h"

using namespace Core;


class ZobFont
{
public:

	ZobFont(const std::string &file, int nbCharWidth, int nbCharHeight);
	ZobFont(const u8* data, int width, int height, int nbCharWidth, int nbCharHeight);
	ZobFont(const float* data, int width, int height, int nbCharWidth, int nbCharHeight);
	~ZobFont();

	const ZobMaterial* GetChar(char c) const;

private:
	void InitFont(const float* data, int width, int height, int nbCharWidth, int nbCharHeight);
	ZobVector3 m_color;
	const ZobMaterial** m_charMaterials;
	int m_nbCharWidth;
	int m_nbCharHeight;
	int m_charWidth;
	int m_charHeight;
};
