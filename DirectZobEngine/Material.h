#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Vector3.h" 
#include "Events.h"

class Material
{
public:

	//Material(const std::string& path, const std::string& file, const std::string& name);
	Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const std::string* textureFile=NULL);
	~Material();
	inline const uint GetWidth() const { return m_width; }
	inline const uint GetHeight() const { return m_height; }
	inline const float* GetData() const { return m_data; }
	inline const Vector3* GetAmbientColor() const { return &m_ambientColor; }
	inline const Vector3* GetDiffuseColor() const { return &m_diffuseColor; }
	inline const std::string& GetName() const { return m_name; }

private:

	float* m_data = NULL;
	uint m_width = 0;
	uint m_height = 0;
	std::string m_name;
	Vector3 m_ambientColor;
	Vector3 m_diffuseColor;
	//const uint* data = &internal_data[0];
};
