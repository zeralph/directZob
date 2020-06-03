#include "Material.h"
#include <iostream>
#include "DirectZob.h"

static int sMaterialNumber = 0;
using namespace directZob;

Material::Material()
{
	sMaterialNumber++;
	m_name = std::string("Material_");
	m_name.append(std::to_string(sMaterialNumber));
	DirectZob::LogInfo("Material %s creation", m_name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = NULL;
	m_ambientColor = Vector3(1, 1, 1);
	m_diffuseColor = Vector3(1, 1, 1);
	DirectZob::RemoveIndent();
}

Material::Material(const std::string& name, const Vector3* ambientColor, const Vector3* diffuseColor, const Texture* texture)
{
	sMaterialNumber++;
	DirectZob::LogInfo("Material %s creation", name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = texture;
	m_name = name;
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	DirectZob::RemoveIndent();
}

Material::~Material()
{
	DirectZob::LogInfo("Delete material %s", m_name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = NULL;
	m_name = std::string("");
	m_ambientColor = Vector3(0, 0, 0);
	m_diffuseColor = Vector3(0, 0, 0);
	DirectZob::RemoveIndent();
}
