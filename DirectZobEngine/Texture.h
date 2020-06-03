#ifndef DZOB_TEXTURE_H
#define DZOB_TEXTURE_H

#include "Types.h"
#include <string>
#include <vector>
#include "Texture.h"
namespace directZob {
class Texture
{
public:
	Texture();
	~Texture();
	void LoadFromFile(const std::string& filePath);

	inline const uint GetWidth() const { return m_width; }
	inline const uint GetHeight() const { return m_height; }
	inline const float* GetData() const { return m_data; }
	inline const uint GetDataSize() const { return m_dataSize; }
	inline const std::string& GetPath() const { return m_fullPath; }

private:

	void OnError(int error, const char* material, const char* texture);

	float* m_data = NULL;
	uint m_dataSize = 0;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_fullPath;

};
}
#endif