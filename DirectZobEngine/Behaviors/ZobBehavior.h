#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "../ZobObjects/ZobEntity.h"
#include "../Misc/ZobVariablesExposer.h"
#include "../Rendering/Engine.h"
#include <map>
#include <vector>
#include <string>

class ZobObject;
class Camera;
class ZobBehavior : public ZobEntity
{
public:

	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_hudElement,
		eBehavior_physicBox,
		eBehavior_physicSphere,
		eBehavior_physicCapsule,
		eBehavior_physicMesh,
		eBehavior_mesh,
		eBehavior_sprite,
		eBehavior_light,
		__eBehavior_MAX__,
	};

	virtual ~ZobBehavior() {};
	virtual void											Init() = 0;
	virtual void											PreUpdate(float dt)=0;
	virtual void											UpdateBeforeObject(float dt) = 0;
	virtual void											UpdateAfterObject(float dt) = 0;
	virtual void											PostUpdate() = 0;
	virtual void											EditorUpdate() = 0;
	virtual void											QueueForDrawing(const Camera* camera, Engine* engine) {};
	virtual	void											DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}

	const char*												GetBehaviorTypeStr();
	TiXmlNode*												SaveUnderNode(TiXmlNode* node);
	eBehaviorType											GetBehaviorType() const { return m_type; }
	void													ReadNode(TiXmlElement* node);
	void													ReLoadVariables();
	ZobVariablesExposer*									GetVariablesExposer() { return m_varExposer; }
protected:
	ZobBehavior(ZobObject* zobObject, bool bEditorZobBehavior);
	eBehaviorType m_type;
	ZobObject* m_zobObject;
	ZobVariablesExposer* m_varExposer;
	
protected:

};