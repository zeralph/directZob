#pragma once
#include "Types.h"
#include "Material.h"
#include "matrix2x2.h"
#include "Engine.h"
#include "Events.h"

using namespace Core;

class Text2D
{
public:

	Text2D(Engine* engine, const Material* tex, int nbCharWidth, int bCharHeight, Events* events);
	~Text2D();
	void Print(uint x, uint y, uint size, const std::string* text, uint color);

private:

	void PrintChar(uint x, uint y, uint size, const char c, uint color);

	Events* m_events;
	const Material* m_texture;
	Engine* m_engine;
	uint m_charWidth;
	uint m_charHeight;
	uint m_nbCharWidth;
	uint m_nbCharHeight;
	Matrix2x2 m_matrix;
};