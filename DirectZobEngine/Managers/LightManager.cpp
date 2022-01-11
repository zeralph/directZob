#include "LightManager.h"
#include "DirectZob.h"
#include "../../dependencies/tinyxml/tinyxml.h"

LightManager::LightManager()
{
	m_varExposer = new ZobVariablesExposer(0);
	m_varExposer->WrapVariable<ZobColor>("Ambient color", &m_ambientColor, NULL, false, true);
	m_varExposer->WrapVariable<ZobColor>("Clear color", &m_clearColor, NULL, false, true);
	m_varExposer->WrapVariable<float>("Ambient color intensity", &m_ambientColorIntensity, NULL, false, true);


	eFogType ft[4] = { eFogType::eFogType_NoFog, eFogType::eFogType_Linear, eFogType::eFogType_Exp, eFogType::eFogType_Exp2 };
	const char* ftStr[4] = { "None", "Linear", "Exp", "Exp2"};
	m_varExposer->WrapEnum<eFogType>("Fog type", &m_fogType, 4, ft, ftStr, NULL, false, false, true);
	m_varExposer->WrapVariable<ZobColor>("Fog color", &m_fogColor, NULL, false, true);
	m_varExposer->WrapVariable<float>("Fog distance", &m_fogDistance, NULL, false, true);
	m_varExposer->WrapVariable<float>("Fog density", &m_fogDensity, NULL, false, true);
	
	ReInitGlobalSettings();
}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
	delete m_varExposer;
}

void LightManager::ReInitGlobalSettings()
{
	m_fogColor = ZobColor(255, 63, 149, 255);
	m_clearColor = ZobColor::Silver;// ZobColor(255, 63, 149, 255);
	m_ambientColor = ZobColor(255, 102, 102, 102);
	m_ambientColorIntensity = 0.8f;
	m_fogDistance = 20.0f;
	m_lights.clear();
	m_lightsToAdd.clear();
	m_lightsToRemove.clear();
	m_fogDensity = 2.0f;
	m_fogType = eFogType::eFogType_NoFog;
	m_lightIndex = 1;
}

void LightManager::Setup(ZobColor* fogColor, ZobColor* ambientColor, ZobColor* clearColor, float fogDistance, float fogDensity, eFogType fogType, float ambientIntensity)
{
	m_fogColor = fogColor;
	m_ambientColor = ambientColor;
	m_clearColor = clearColor;
	m_fogDistance = fogDistance;
	m_fogDensity = fogDensity;
	m_fogType = fogType;
	m_ambientColorIntensity = ambientIntensity;
}

void LightManager::PreUpdate(float dt)
{
	for (std::vector<Light*>::const_iterator iter = m_lightsToAdd.begin(); iter != m_lightsToAdd.end(); iter++)
	{
		m_lights.push_back(*iter);
		m_lightIndex++;
	}
	m_lightsToAdd.clear();
	for (std::vector<Light*>::const_iterator iter = m_lightsToRemove.begin(); iter != m_lightsToRemove.end(); iter++)
	{
		Light* toRemove = (*iter);
		for (int i = 0; i < m_lights.size(); i++)
		{
			if (m_lights[i] == toRemove)
			{
				std::swap(m_lights.at(i), m_lights.at(m_lights.size() - 1));
				m_lights.pop_back();
				m_lightIndex--;
			}
		}
	}
	m_lightsToRemove.clear();
}

void LightManager::RemoveLight(Light* l)
{
	m_lightsToRemove.push_back(l);
}

void LightManager::UnloadAll()
{
	//for (int i = 0; i < m_lights.size(); i++)
	//{
	//	delete (m_lights[i]);
	//}
	m_lights.clear();
}

Light* LightManager::CreateLight(std::string& name, Light::eLightType type, ZobVector3 position, ZobColor color, float intensity, float distance, ZobObject* parent)
{
	Light* l = new Light(name, type, color, intensity, distance, parent);
	l->SetWorldPosition(position.x, position.y, position.z);
	if (type == Light::eLightType_spot)
	{
		l->SetWorldRotation(90, 0, 0);
	}
	else if (type == Light::eLightType_directional)
	{
		l->SetWorldRotation(-45, -45, -45);
	}
	AddLight(l);
	return l;
}

void LightManager::AddLight(Light* l)
{
	m_lightsToAdd.push_back(l);
}

Light* LightManager::CreateLight(Light::eLightType type)
{
	int l = m_lightIndex;
	std::string name = std::string("Light_").append(std::to_string((l)));
	ZobColor color = ZobColor(255, 252, 212, 64);
	return CreateLight(name, type, ZobVector3(0, 10, 0), color, 5.0f, 100.0f, NULL);
}

Light* LightManager::GetLight(const std::string& name) const
{
	for (std::vector<Light*>::const_iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}

const std::vector<const Light*>* LightManager::GetActiveLights() const
{
	return &m_activeLights;
}

void LightManager::Update()
{
	m_activeLights.clear();
	if (m_lightingEnabled)
	{
		for (std::vector<Light*>::const_iterator iter = m_lights.begin(); iter != m_lights.end(); iter++)
		{
			if ((*iter)->IsActive())
			{
				m_activeLights.push_back((*iter));
			}
		}
	}
}

void LightManager::LoadFromNode(TiXmlElement* node)
{
	if (node)
	{
		m_varExposer->ReadNode(node);
	}
}

void LightManager::SaveUnderNode(TiXmlElement* node)
{
	m_varExposer->SaveUnderNode(node);
}
