#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Events.h"

class Texture
{
public:

	Texture(const std::string& path, const std::string& file, const std::string& name);

	inline const uint GetWidth() const { return m_width; }
	inline const uint GetHeight() const { return m_height; }
	inline const float* GetData() const { return data; }
	inline const std::string& GetName() const { return m_name; }
	inline const std::string& GetFile() const { return m_file; }
private:

	float* data = NULL;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_file;
	std::string m_name;
	//const uint* data = &internal_data[0];
};