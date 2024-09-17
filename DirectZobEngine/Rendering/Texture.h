#pragma once
#include "../Types.h"
#include <string>
#include <vector>

class ZobFilePath;
class Texture
{
public:
	Texture();
	~Texture();
	void Load(ZobFilePath* zfp);
	void LoadFromData(std::string& name, int w, int h, float* data);

	inline const uint GetWidth() const { return m_width; }
	inline const uint GetHeight() const { return m_height; }
	inline const float* GetData() const { return m_data; }
	inline const uint GetDataSize() const { return m_dataSize; }
	inline const std::string& GetName() const { return m_name; }

private:

	void OnError(int error, const char* material, const char* texture);

	float* m_data = NULL;
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_name;

};