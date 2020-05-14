#include "Material.h"
#include <iostream>
#include "DirectZob.h"

Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const Texture* texture)
{
	DirectZob::LogInfo("Material %s creation", name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = texture;
	m_name = name.c_str();
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	DirectZob::RemoveIndent();
}

Material::~Material()
{
	DirectZob::LogInfo("Delete material %s", m_name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = NULL;
	m_name = "deleted";
	m_ambientColor = Vector3(0, 0, 0);
	m_diffuseColor = Vector3(0,0,0);
	DirectZob::RemoveIndent();
}
