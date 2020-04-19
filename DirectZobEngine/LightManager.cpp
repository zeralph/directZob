#include "LightManager.h"
#include "DirectZob.h"

LightManager::LightManager()
{
	m_lights.clear();
	std::string l = "light1";
	Vector3 c = Vector3(94.5f / 255.0f, 85.5f / 255.0f, 64.3f / 255.0f);


	c = Vector3(1.0f, 0.2f, 0.2f);
	CreateLight(l, Vector3(-25, 20, 0), Vector3(-1, -1, -1), c, 1, 150);
	l = "light2";
	c = Vector3(0.0f, 0.2f, 1.0f);
	CreateLight(l, Vector3(25, 20, 0), Vector3(-1, -1, -1), c, 1, 150);

	m_ambientColor = Vector3(0.4f, 0.4f, 0.4f);

}

LightManager::~LightManager()
{
	for (int i = 0; i < m_lights.size(); i++)
	{
		delete (m_lights[i]);
	}
}

void LightManager::CreateLight(std::string& name, Vector3 position, Vector3 orientation, Vector3 color, float intensity, float distance)
{
	Light* l = new Light(name, color, intensity, distance);
	l->SetTranslation(position.x, position.y, position.z);
	l->SetRotation(orientation.x, orientation.y, orientation.z);
	m_lights.push_back(l);
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