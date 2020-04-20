#include "LightManager.h"
#include "DirectZob.h"

LightManager::LightManager()
{
	m_lights.clear();
	
	Vector3 c = Vector3(94.5f / 255.0f, 85.5f / 255.0f, 64.3f / 255.0f);

	std::string l = "lightRed";
	c = Vector3(1.0f, 0.0f, 0.0f);
	CreatePointLight(l, Vector3(50, 0, 0), c, 1.0f, 500, NULL);

	l = "lightGreen";
	c = Vector3(0.0f, 1.0f, 0.0f);
	CreatePointLight(l, Vector3(0, 50, 0), c, 1.0f, 500, NULL);

	l = "lightBlue";
	c = Vector3(0.0f, 0.0f, 1.0f);
	CreatePointLight(l, Vector3(0, 0, 50), c, 1.0f, 500, NULL);

	m_ambientColor = Vector3(0.4f, 0.4f, 0.4f);

}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
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