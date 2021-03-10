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
	m_ambientColor = ZobVector3(1, 1, 1);
	m_diffuseColor = ZobVector3(1, 1, 1);
	m_specularColor = ZobVector3(1, 1, 1);
	m_specularExponent = 0.0f;
	DirectZob::RemoveIndent();
}

ZobMaterial::ZobMaterial(const std::string& name, const ZobVector3* ambientColor, const ZobVector3* diffuseColor, const ZobVector3* specularColor, float specularExponent, float transparency, const Texture* texture)
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
	m_ambientColor = ZobVector3(0, 0, 0);
	m_diffuseColor = ZobVector3(0,0,0);
	DirectZob::RemoveIndent();
}
