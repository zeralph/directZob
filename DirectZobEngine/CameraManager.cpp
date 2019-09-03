#include "CameraManager.h"
#include "DirectZob.h"

CameraManager::CameraManager()
{
	m_cameras.clear();
	std::string n = "Freecam";
	LoadCamera(n, Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(0, 1, 0), 45.0f);
}

CameraManager::~CameraManager()
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		delete (m_cameras[i]);
	}
	m_cameras.clear();
}

void CameraManager::LoadCamera(std::string& name, Vector3 position, Vector3 target, Vector3 up, float fov)
{
	if (GetCamera(name) == NULL)
	{
		Camera* c = new Camera(name, position, target, up, fov, DirectZob::GetInstance()->GetEngine()->GetBufferData());
		m_cameras.push_back(c);
	}
	else
	{
		//output error
	}
}

Camera* CameraManager::GetCamera(std::string& name) const
{
	for (std::vector<Camera*>::const_iterator iter = m_cameras.begin(); iter != m_cameras.end(); iter++)
	{
		if ((*iter)->GetName() == name)
		{
			return *iter;
		}
	}
	return NULL;
}