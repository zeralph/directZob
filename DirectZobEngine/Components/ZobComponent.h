#pragma once
#include "../Types.h"
#include "../ZobObjects/ZobEntity.h"
#include "ZobComponentFactory.h"
#include <map>
#include <vector>
#include <string>

class TiXmlNode;
class TiXmlElement;
class ZobObject;
class Camera;
class Engine;
class ZobVariablesExposer;
class ZobComponent : public ZobEntity
{
public:

	virtual ~ZobComponent() {};
	virtual void											Init(DirectZobType::sceneLoadingCallback cb) override = 0;
	virtual void											PreUpdate(float dt)=0;
	virtual void											PostUpdate() = 0;
	virtual void											EditorUpdate() = 0;
	virtual void											QueueForDrawing(const Camera* camera, Engine* engine) {};
	virtual	void											DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}
	void													Duplicate() override;
	const char*												GetComponentTypeStr();
	TiXmlNode*												SaveUnderNode(TiXmlNode* node);
	ZobComponentFactory::eComponentType						GetComponentType() const { return m_componentType; }
	void													ReadNode(TiXmlElement* node);
	void													ReLoadVariables();
	ZobVariablesExposer*									GetVariablesExposer() { return m_varExposer; }
	bool													IsEnabled() const { return m_enabled; }
	void													Enable(bool b) { m_enabled = b; }
protected:
	ZobComponent(ZobObject* zobObject, bool bEditorZobComponent);
	ZobComponentFactory::eComponentType						m_componentType;
	ZobObject*												m_zobObject;
	ZobVariablesExposer*									m_varExposer;
	bool													m_enabled;
protected:

};