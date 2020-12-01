#pragma once
#include "../Types.h"
#include "../ZobObjects/Camera.h"
#include <string>
#include <vector>

class ZobObject;
class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	Camera* 						CreateCamera(ZobObject* parent);
	Camera* 						CreateEditorCamera();
	Camera* 						CreateCamera(std::string& name, Camera::eCameraType type, float fov, ZobObject* parent);
	Camera*					 		GetCurrentCamera() {return m_curCam;}
	Camera* 						GetCamera(std::string& name) const;
	void 							AddCamera(Camera* c);
	void 							UpdateAfter();
	void 							SetNextCamera(std::string& name);
	void 							RemoveCamera(Camera* c);
	const std::vector<std::string> 	GetCameraList() const;
	void							UnloadAll();

private:
	std::vector<Camera*> m_cameras;
	Camera* m_curCam = NULL;
	Camera* m_nextCamera = NULL;
};