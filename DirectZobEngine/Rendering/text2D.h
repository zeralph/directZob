#pragma once
#include "../Types.h"
#include "../ZobMaterial.h"
#include "ZobMatrix2x2.h"
#include "Engine.h"
#include "../Events.h"

class Text2D
{
public:

	Text2D(Engine* engine, Events* events);
	~Text2D();
	void Print(uint x, uint y, const std::string* text, uint color);
	void Print(uint x, uint y, uint color, const char* fmt, ...);
	void GetChar(char c, ZobTexture &tex) const;
private:

	void PrintChar(uint x, uint y, const char c, uint color);

	Engine* m_engine;
	Events* m_events;
	const u8* m_data;
	uint m_texWidth;
	uint m_texHeight;
	uint m_charWidth;
	uint m_charHeight;
	uint m_nbCharWidth;
	uint m_nbCharHeight;
	ZobMatrix2x2 m_matrix;
	static char m_buffer[4096];
};
