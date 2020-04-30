#include "LightManager.h"
#include "DirectZob.h"

LightManager::LightManager()
{
	m_fogColor = Vector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_clearColor = Vector3(63.0f / 255.0f, 149.0f / 255.0f, 255.0f / 255.0f);
	m_ambientColor = Vector3(0.4f, 0.4f, 0.4f);
	m_fogDistance = 500.0f;
	m_lights.clear();
	m_fogDensity = 2.0f;
	m_fogType = FogType::FogType_Exp;
	
	Vector3 c = Vector3(94.5f / 255.0f, 85.5f / 255.0f, 64.3f / 255.0f);
/*
	std::string l = "lightRed";
	c = Vector3(1.0f, 0.0f, 0.0f);
	CreatePointLight(l, Vector3(50, 0, 0), c, 1.0f, 500, nullptr);

	l = "lightGreen";
	c = Vector3(0.0f, 1.0f, 0.0f);
	CreatePointLight(l, Vector3(0, 50, 0), c, 1.0f, 500, nullptr);

	l = "lightBlue";
	c = Vector3(0.0f, 0.0f, 1.0f);
	CreatePointLight(l, Vector3(0, 0, 50), c, 1.0f, 500, nullptr);
*/
}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
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
	m_lights.push_back(l);
	return l;
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
	return &m_lights;
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
