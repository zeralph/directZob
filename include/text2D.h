#pragma once
#include "Types.h"
#include "Texture.h"
#include "matrix2x2.h"
#include "Engine.h"

class Text2D
{
public:

	Text2D(Engine* engine, const Texture* tex, int nbCharWidth, int bCharHeight);
	~Text2D();
	void Print(uint x, uint y, uint size, const std::string* text, uint color);

private:

	void PrintChar(uint x, uint y, uint size, const char c, uint color);

	const Texture* m_texture;
	Engine* m_engine;
	uint m_charWidth;
	uint m_charHeight;
	uint m_nbCharWidth;
	uint m_nbCharHeight;
	Matrix2x2 m_matrix;
};