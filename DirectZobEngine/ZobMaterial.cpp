#include "ZobMaterial.h"
#include <iostream>
#include "DirectZob.h"

static int sMaterialNumber = 0;
ZobMaterial::ZobMaterial()
{
	sMaterialNumber++;
	m_name = std::string("Material_");
	m_name.append(std::to_string(sMaterialNumber));
	//DirectZob::LogInfo("Material %s creation", m_name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = NULL;
	m_ambientColor = ZobColor(255, 255, 255, 255);
	m_diffuseColor = ZobColor(255, 255, 255, 255);
	m_specularColor = ZobColor(255, 255, 255, 255);
	m_specularExponent = 0.0f;
	DirectZob::RemoveIndent();
}

ZobMaterial::ZobMaterial(const std::string& name, const ZobColor* ambientColor, const ZobColor* diffuseColor, const ZobColor* specularColor, float specularExponent, float transparency, const Texture* texture)
{
	sMaterialNumber++;
	//DirectZob::LogInfo("Material %s creation", name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = texture;
	m_name = name;
	m_ambientColor = ambientColor;
	m_diffuseColor = diffuseColor;
	m_specularColor = specularColor;
	m_specularExponent = specularExponent;
	DirectZob::RemoveIndent();
}

ZobMaterial::~ZobMaterial()
{
	//DirectZob::LogInfo("Delete material %s", m_name.c_str());
	DirectZob::AddIndent();
	m_diffuseTexture = NULL;
	m_name = std::string("");
	m_ambientColor = ZobColor(255, 255, 255, 255);
	m_diffuseColor = ZobColor(255, 255, 255, 255);
	DirectZob::RemoveIndent();
}
