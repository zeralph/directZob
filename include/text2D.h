#pragma once
#include "Types.h"

class Text2D
{
public:
	static uint* GetCharBuffer(char c);
	static int GetCharWidth();
	static int GetCharHeight();
private:
	static const uint* m_charBuffer;
};