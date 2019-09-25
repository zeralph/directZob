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
	bool Reparent(ZobObject *o, ZobObject* parent);
private:
	void GetZobObjectListInternal(const ZobObject* z, std::string& str);
	ZobObject* LoadEditorMesh(const char* name, const char* meshPath, const char* meshFile, ZobObject* parent);
	ZobObject* GetZobObjectFromPartialId(ZobObject* z, const uint id) const;
	ZobObject* m_rootObject = NULL;

//Editor objects
	ZobObject* m_editorGizmos = NULL;
	ZobObject* m_transform = NULL;
	ZobObject* m_transformX = NULL;
	ZobObject* m_transformY = NULL;
	ZobObject* m_transformZ = NULL;
	ZobObject* m_rotate = NULL;
	ZobObject* m_rotateX = NULL;
	ZobObject* m_rotateY = NULL;
	ZobObject* m_rotateZ = NULL;
	ZobObject* m_scale = NULL;
	ZobObject* m_scaleX = NULL;
	ZobObject* m_scaleY = NULL;
	ZobObject* m_scaleZ = NULL;
}; 
