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
class ZobComponent : public ZobEntity
{
public:

	enum eComponentType
	{
		eComponent_none = 0,
		eComponent_car,
		eComponent_hudElement,
		eComponent_physicBox,
		eComponent_physicSphere,
		eComponent_physicCapsule,
		eComponent_physicMesh,
		eComponent_mesh,
		eComponent_sprite,
		eComponent_light,
		__eComponent_MAX__,
	};

	virtual ~ZobComponent() {};
	virtual void											Init() = 0;
	virtual void											PreUpdate(float dt)=0;
	virtual void											UpdateBeforeObject(float dt) = 0;
	virtual void											UpdateAfterObject(float dt) = 0;
	virtual void											PostUpdate() = 0;
	virtual void											EditorUpdate() = 0;
	virtual void											QueueForDrawing(const Camera* camera, Engine* engine) {};
	virtual	void											DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}

	const char*												GetComponentTypeStr();
	TiXmlNode*												SaveUnderNode(TiXmlNode* node);
	eComponentType											GetComponentType() const { return m_type; }
	void													ReadNode(TiXmlElement* node);
	void													ReLoadVariables();
	ZobVariablesExposer*									GetVariablesExposer() { return m_varExposer; }
protected:
	ZobComponent(ZobObject* zobObject, bool bEditorZobComponent);
	eComponentType m_type;
	ZobObject* m_zobObject;
	ZobVariablesExposer* m_varExposer;
	
protected:

};