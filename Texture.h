#pragma once
#include "Types.h"
class Texture
{
public:
	virtual const uint GetWidth() const { return width; }
	virtual const uint GetHeight() const { return height; }
	virtual const uint* GetData() const = 0;

	const uint width = 64;
	const uint height = 64;
	//const uint* data = NULL;
	//const uint* data = &internal_data[0];
};