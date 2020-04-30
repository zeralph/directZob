#include "Material.h"
#include <iostream>
#include "DirectZob.h"

Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const Texture* texture)
{
	m_diffuseTexture = texture;
	m_name = name.c_str();
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	DirectZob::LogInfo("Loaded material %s", name.c_str());
}

Material::~Material()
{
	m_diffuseTexture = NULL;
	m_name = "deleted";
	m_ambientColor = Vector3(0, 0, 0);
	m_diffuseColor = Vector3(0,0,0);
}
