#pragma once
#include "../Types.h"
#include "../ZobObjects/ZobObject.h"
#include <string>
#include <vector>
#include "../ZobObjects/Camera.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/Light.h"

class ZobObjectManager
{
public:
	ZobObjectManager();
	~ZobObjectManager();

	//friend void ZobObject::SetParent(ZobObject* o);


	void										UpdateObjects(const Camera* camera, Engine* engine, float dt);
	void										Init(DirectZobType::sceneLoadingCallback cb);
	void										PreUpdate(float dt);
	void										PostUpdate();
	void										EditorUpdate();
	void										QueueForDrawing(const Camera* camera, Engine* engine);
	void										AddZobObject(ZobObject* z);
	ZobObject*									GetRootObject() const;
	ZobObject*									GetZobObject(const std::string& name) const;
	void										GetZobObjectList(std::string &s) const;
	ZobObject*									GetRootObject() { return m_rootObject;  }
	ZobObject*									CreateZobObject(ZobObject* parent);
	void										RemoveZobObject(ZobObject* z);
	void										UnloadAll();
	bool										Reparent(ZobObject* o, ZobObject* parent);
	void										SaveTransforms();
	void										DrawGizmos(const Camera* camera, Engine* engine);
	ZobObject*									CreateEditorZobObject(ZobObject* parent);
	void										RestoreTransforms();
	void										ResetPhysic();
	void										AddIdToDeleted(ulong id) { m_deletedIds.push_back(id); }
	bool										IsDeleted(ulong id);
	void										GetZobObjectList(std::vector<const ZobObject*>& v) const;

private:
	void										GetZobObjectListInternal(const ZobObject* z, std::string& str) const;
	void										GetZobObjectListInternal(const ZobObject* z, std::vector<const ZobObject*>& v) const;
	ZobObject*									m_rootObject = NULL;
	clock_t										m_drawTick;
	float										m_time;
	std::vector<ulong>							m_deletedIds;
}; 
