#include "CameraManager.h"
#include "DirectZob.h"

CameraManager::CameraManager()
{
	m_cameras.clear();
	std::string n = "EditorCam";
	CreateCamera(n, Vector3(50, -50, 50), Vector3(0, 0, 0), Vector3(0, 1, 0), 45.0f);
	m_curCam = m_cameras[0];
}

CameraManager::~CameraManager()
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		delete (m_cameras[i]);
	}
	m_cameras.clear();
}

void CameraManager::CreateCamera(std::string& name, Vector3 position, Vector3 target, Vector3 up, float fov)
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

const std::vector<std::string>& CameraManager::GetCameraList() const 
{
	std::vector<std::string> vec;
	vec.clear();
	for (std::vector<Camera*>::const_iterator iter = m_cameras.begin(); iter != m_cameras.end(); iter++)
	{
		vec.push_back((*iter)->GetName());
	}
	return vec;
}