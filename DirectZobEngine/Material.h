#pragma once
#include "Types.h"
#include "lodepng.h"
#include <string>
#include "Vector3.h" 
#include "Events.h"
#include "Texture.h"

class Material
{
public:

	//Material(const std::string& path, const std::string& file, const std::string& name);
	Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const Texture* texture);
	~Material();
	inline const Texture* GetDiffuseTexture() const { return m_diffuseTexture; }
	inline const Vector3* GetAmbientColor() const { return &m_ambientColor; }
	inline const Vector3* GetDiffuseColor() const { return &m_diffuseColor; }
	inline const std::string& GetName() const { return m_name; }

private:

	const Texture* m_diffuseTexture;
	std::string m_name;
	Vector3 m_ambientColor;
	Vector3 m_diffuseColor;
	//const uint* data = &internal_data[0];
};
