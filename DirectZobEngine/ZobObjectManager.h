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

	//friend void ZobObject::SetParent(ZobObject* o);

	void UpdateObjects();
	void DrawObjects(const Camera* camera, Core::Engine* engine);
	ZobObject* GetZobObjectFromPartialId(const uint id) const;
	void AddZobObject(ZobObject* z);
	ZobObject* GetRootObject() const;
	ZobObject* GetZobObject(const std::string& name) const;
	std::string GetZobObjectList();
	ZobObject* GetRootObject() { return m_rootObject;  }
	void CreateZobObject(ZobObject* parent);
	void RemoveZobObject(ZobObject* z);
	void UnloadAll();
	void CreateEditorGizmos(std::string& editorResourcesPath);
	ZobObject* GetEditorGizmos() { return m_editorGizmos; }
	bool Reparent(ZobObject* o, ZobObject* parent);
private:
	void GetZobObjectListInternal(const ZobObject* z, std::string& str);
	ZobObject* LoadEditorMesh(const char* name, const char* meshPath, const char* meshFile, ZobObject* parent);
	ZobObject* GetZobObjectFromPartialId(ZobObject* z, const uint id) const;
	ZobObject* m_rootObject = nullptr;

//Editor objects
	ZobObject* m_editorGizmos = nullptr;
	ZobObject* m_transform = nullptr;
	ZobObject* m_transformX = nullptr;
	ZobObject* m_transformY = nullptr;
	ZobObject* m_transformZ = nullptr;
	ZobObject* m_rotate = nullptr;
	ZobObject* m_rotateX = nullptr;
	ZobObject* m_rotateY = nullptr;
	ZobObject* m_rotateZ = nullptr;
	ZobObject* m_scale = nullptr;
	ZobObject* m_scaleX = nullptr;
	ZobObject* m_scaleY = nullptr;
	ZobObject* m_scaleZ = nullptr;
}; 
