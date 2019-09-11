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
	std::string GetZobObjectList();
	ZobObject* GetRootObject() { return m_rootObject;  }
	void CreateZobObject(ZobObject* parent);
	void RemoveZobObject(ZobObject* z);
	
private:
	void GetZobObjectListInternal(const ZobObject* z, std::string& str);
	ZobObject* m_rootObject = NULL;
}; 
