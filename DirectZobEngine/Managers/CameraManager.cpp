#include "CameraManager.h"
#include "DirectZob.h"

CameraManager::CameraManager()
{
	m_cameras.clear();
	m_curCam = NULL;
	m_nextCamera = NULL;
}

CameraManager::~CameraManager()
{
//	for (int i = 0; i < m_cameras.size(); i++)
//	{
//		delete (m_cameras[i]);
//	}
	m_cameras.clear();
}

void CameraManager::UnloadAll()
{
	m_cameras.clear();
}

void CameraManager::RemoveCamera(Camera* c)
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		if (m_cameras[i] == c)
		{
			std::swap(m_cameras.at(i), m_cameras.at(m_cameras.size() - 1));
			m_cameras.pop_back();
		}
		if (c == m_curCam)
		{
			m_curCam = NULL;
		}
		if (c == m_nextCamera)
		{
			m_nextCamera = NULL;
		}
	}
}

Camera* CameraManager::CreateEditorCamera()
{
	std::string s = std::string("EditorCamera");
	Camera* c = GetCamera(s);
	if (c)
	{
		return c;
	}
	else
	{
		std::string name = std::string("EditorCamera");
		c = DirectZob::GetInstance()->GetCameraManager()->CreateCamera(name, Camera::eCamera_orbital, 45.0f, NULL);
		c->ChangeType(ZOBGUID::type_editor);
		c->SetWorldPosition(-20,20,-20);

		//c->SetPosition(0, 10, -10);

		ZobVector3 v = ZobVector3(0, 0, 0);
		c->SetTarget(&v);
		//c->SetRotation(-20, 0, 0);
		
		return c;
	}
}

void CameraManager::UpdateAfter()
{
	if (m_nextCamera)
	{
		m_curCam = m_nextCamera;
		m_nextCamera = NULL;
	}
}

Camera* CameraManager::CreateCamera(ZobObject* parent)
{
	int l = m_cameras.size();
	std::string name = std::string("Camera_").append(std::to_string((l)));
	Camera* c = DirectZob::GetInstance()->GetCameraManager()->CreateCamera(name, Camera::eCamera_fps, 45.0f, parent);
	c->SetWorldPosition(0, 5, 0);
	return c;
}

void CameraManager::SetNextCamera(std::string& name)
{
	for (int i = 0; i < m_cameras.size(); i++)
	{
		if (m_cameras[i]->GetName() == name)
		{
			m_nextCamera = m_cameras[i];
			return;
		}
	}
}

void CameraManager::AddCamera(Camera* c)
{
		m_cameras.push_back(c);
		if (!m_curCam)
		{
			m_curCam = c;
		}
}

Camera* CameraManager::CreateCamera(std::string& name, Camera::eCameraType type, float fov, ZobObject* parent)
{
	if (GetCamera(name) == NULL)
	{
		Camera* c = new Camera(name, type, fov, DirectZob::GetInstance()->GetEngine()->GetBufferData(), parent);
		AddCamera(c);
		return c;
	}
	else
	{
		return NULL;
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

const std::vector<std::string> CameraManager::GetCameraList() const 
{
	std::vector<std::string> vec;
	vec.clear();
	for (std::vector<Camera*>::const_iterator iter = m_cameras.begin(); iter != m_cameras.end(); iter++)
	{
		vec.push_back((*iter)->GetName());
	}
	return vec;
}