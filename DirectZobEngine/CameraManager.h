#pragma once
#include "Types.h"
#include "Camera.h"
#include <string>
#include <vector>

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	Camera* CreateCamera();
	Camera* CreateCamera(std::string& name, Vector3 position, Vector3 target, Vector3 up, float fov);
	Camera* GetCurrentCamera() {return m_curCam;}
	Camera* GetCamera(std::string& name) const;
	void SetCurrentCamera(std::string& name);
	void RemoveCamera(Camera* c);
	const std::vector<std::string> GetCameraList() const;

private:
	std::vector<Camera*> m_cameras;
	Camera* m_curCam;
};