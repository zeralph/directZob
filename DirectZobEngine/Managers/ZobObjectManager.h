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


	void UpdateObjects(const Camera* camera, Engine* engine, float dt);
	void Init();
	void PreUpdate(float dt);
	void PostUpdate();
	void EditorUpdate();
	void QueueForDrawing(const Camera* camera, Engine* engine);
	ZobObject* GetZobObjectFromlId(const zobId id) const;
	ZobObject* GetZobObjectFromlId(const std::string& id) const;
	void AddZobObject(ZobObject* z);
	ZobObject* GetRootObject() const;
	ZobObject* GetZobObject(const std::string& name) const;
	void GetZobObjectList(std::string &s);
	void GetZobObjectList(std::vector<const ZobObject*> &v);
	ZobObject* GetRootObject() { return m_rootObject;  }
	ZobObject* CreateZobObject(ZobObject* parent);
	ZobObject* CreateEditorZobObject(ZobObject* parent);
	ZobSprite* CreateZobSprite(ZobObject* parent);
	void RemoveZobObject(ZobObject* z);
	void UnloadAll();
	bool Reparent(ZobObject* o, ZobObject* parent);
	void SaveTransforms();
	void RestoreTransforms();
	void ResetPhysic();
	void AddIdToDeleted(ulong id) { m_deletedIds.push_back(id); }
	bool IsDeleted(ulong id);
private:
	void GetZobObjectListInternal(const ZobObject* z, std::string& str);
	void GetZobObjectListInternal(const ZobObject* z, std::vector<const ZobObject*>& v);
	ZobObject* GetZobObjectFromId(ZobObject* z, const zobId id) const;
	ZobObject* m_rootObject = nullptr;
	clock_t	m_drawTick;
	float m_time;
	std::vector<ulong> m_deletedIds;
}; 
