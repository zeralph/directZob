#include "LightManager.h"
#include "DirectZob.h"

LightManager::LightManager()
{
	ReInitGlobalSettings();
}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
}

void LightManager::ReInitGlobalSettings()
{
	m_fogColor = Vector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_clearColor = Vector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_ambientColor = Vector3(0.4f, 0.4f, 0.4f);
	m_fogDistance = 500.0f;
	m_lights.clear();
	m_fogDensity = 2.0f;
	m_fogType = FogType::FogType_NoFog;
}

void LightManager::Setup(Vector3* fogColor, Vector3* ambientColor, Vector3* clearColor, float fogDistance, float fogDensity, FogType fogType)
{
	m_fogColor = fogColor;
	m_ambientColor = ambientColor;
	m_clearColor = clearColor;
	m_fogDistance = fogDistance;
	m_fogDensity = fogDensity;
	m_fogType = fogType;
}

void LightManager::RemoveLight(Light* l)
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		if (m_lights[i] == l)
		{
			std::swap(m_lights.at(i), m_lights.at(m_lights.size() - 1));
			m_lights.pop_back();
		}
	}
}

void LightManager::UnloadAll()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
	m_lights.clear();
}

Light* LightManager::CreatePointLight(std::string& name, Vector3 position, Vector3 color, float intensity, float distance, ZobObject* parent)
{
	Light* l = new Light(name, color, intensity, distance, parent);
	l->SetTranslation(position.x, position.y, position.z);
	l->SetRotation(0,0,0);
	AddLight(l);
	return l;
}

void LightManager::AddLight(Light* l)
{
	m_lights.push_back(l);
}

Light* LightManager::CreateLight()
{
	int l = m_lights.size();
	std::string name = std::string("Light_").append(std::to_string((l)));
	Vector3 color = Vector3(252.0f / 255.0f, 212 / 255.0f, 64.0f / 255.0f);
	return CreatePointLight(name, Vector3(0, 10, 0), color, 5.0f, 100.0f, NULL);
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

const std::vector<Light*>* LightManager::GetActiveLights() const
{
	if(m_lightingEnabled)
	{
		return &m_lights;
	}
	else
	{
		return NULL;
	}
	/*
	if (m_lights.size() > 0)
	{
		Light* l = m_lights[0];
		Vector3 v =l->GetTransform() - position;
		float d = v.sqrtLength();
		for (int i = 1; i < m_lights.size(); i++)
		{
			v = m_lights[i]->GetTransform()
				- position;
			float d2 = v.sqrtLength();
			if (d2 < d)
			{
				d = d2;
				l = m_lights[i];
			}
		}
		return l;
	}
	return NULL;
	*/
}
