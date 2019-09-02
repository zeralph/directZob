#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Events.h"

class Texture
{
public:

	Texture(const char* path, Events* events);

	inline const uint GetWidth() const { return m_width; }
	inline const uint GetHeight() const { return m_height; }
	inline const float* GetData() const { return data; }

private:
	Events* m_events;
	uint m_width = 0;
	uint m_height = 0;
	float* data = NULL;
	//const uint* data = &internal_data[0];
};