#pragma once
#include "../Types.h"
#include "../ZobObjects/ZobObject.h"
#include <string>
#include <vector>
#include "../ZobObjects/Camera.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/ZobSprite.h"

class ZobObjectManager
{
public:
	ZobObjectManager();
	~ZobObjectManager();

	//friend void ZobObject::SetParent(ZobObject* o);


	void StartUpdateScene(const Camera* camera, Core::Engine* engine);
	float WaitForUpdateObjectend();
	void QueueForDrawing(const Camera* camera, Core::Engine* engine);
	void PreUpdate();
	void UpdateBehavior();
	ZobObject* GetZobObjectFromPartialId(const uint id) const;
	void AddZobObject(ZobObject* z);
	ZobObject* GetRootObject() const;
	ZobObject* GetZobObject(const std::string& name) const;
	std::string GetZobObjectList();
	ZobObject* GetRootObject() { return m_rootObject;  }
	ZobObject* CreateZobObject(ZobObject* parent);
	ZobSprite* CreateZobSprite(ZobObject* parent);
	void RemoveZobObject(ZobObject* z);
	void UnloadAll();
	void CreateEditorGizmos(std::string& editorResourcesPath);
	bool Reparent(ZobObject* o, ZobObject* parent);
	void SaveTransforms();
	void RestoreTransforms();
	void ResetPhysic();
	void AddIdToDeleted(ulong id) { m_deletedIds.push_back(id); }
	bool IsDeleted(ulong id);
private:
	void UpdateObjects(const Camera* camera, Core::Engine* engine);
	void GetZobObjectListInternal(const ZobObject* z, std::string& str);
	ZobObject* LoadEditorMesh(const char* name, const char* meshPath, const char* meshFile, ZobObject* parent);
	ZobObject* GetZobObjectFromPartialId(ZobObject* z, const uint id) const;
	ZobObject* m_rootObject = nullptr;
	clock_t	m_drawTick;
	float m_time;
	std::vector<ulong> m_deletedIds;
}; 