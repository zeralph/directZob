#pragma once
#include "../Types.h"
#include "../ZobMaterial.h"
#include "ZobMatrix2x2.h"
#include "Engine.h"
#include "../Events.h"
#include "../ZobMaterial.h"

using namespace Core;


class Font
{
public:

	Font(const std::string &file, int nbCharWidth, int nbCharHeight);
	~Font();

	const ZobMaterial* GetChar(char c) const;

private:

	Texture m_baseTexture;
	ZobVector3 m_color;
	const ZobMaterial** m_charMaterials;
	int m_nbCharWidth;
	int m_nbCharHeight;
	int m_charWidth;
	int m_charHeight;
};
