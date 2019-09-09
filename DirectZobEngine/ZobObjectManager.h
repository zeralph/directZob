#pragma once
#include "Types.h"
#include "ZobObject.h"
#include <string>
#include <vector>
#include "Camera.h"
#include "Engine.h"

class ZobObjectManager
{
public:
	ZobObjectManager();
	~ZobObjectManager();

	void UpdateObjects();
	void DrawObjects(const Camera* camera, Core::Engine* engine);

	void AddZobObject(ZobObject* z);
	ZobObject* GetZobObject(const std::string& name) const;

private:
	std::vector<ZobObject*> m_zobObjects;

}; 
