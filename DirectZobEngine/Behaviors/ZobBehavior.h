#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include <map>

class ZobObject;
class Camera;
class ZobBehavior
{
public:
	enum eBehaviorType
	{
		eBehavior_none = 0,
		eBehavior_car,
		eBehavior_menu,
	};
	virtual ~ZobBehavior() {};
	virtual TiXmlNode*	SaveUnderNode(TiXmlNode* node)=0;
	virtual void		PreUpdate()=0;
	virtual void		Init()=0;
	virtual void		Update(float dt) = 0;
	eBehaviorType		GetBehaviorType() const { return m_type; }
	virtual const char* GetBehaviorTypeStr()=0;
	virtual	void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const {}
protected:
	ZobBehavior(ZobObject* zobObject, TiXmlElement* node) { m_zobObject = zobObject;  m_type = eBehavior_none; };
	eBehaviorType m_type;
	ZobObject* m_zobObject;
private:

};