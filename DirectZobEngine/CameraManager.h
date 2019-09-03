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

	void CreateCamera(std::string& name, Vector3 position, Vector3 target, Vector3 up, float fov);
	Camera* GetCurrentCamera() {return m_curCam;}
	Camera* GetCamera(std::string& name) const;

private:
	std::vector<Camera*> m_cameras;
	Camera* m_curCam;
};