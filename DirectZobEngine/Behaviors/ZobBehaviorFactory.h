#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"


class ZobObject;
class ZobBehaviorFactory 
{
public:

	static const char* eBehaviorTypeStr[ZobBehavior::__eBehavior_MAX__];
	static ZobBehavior* CreateBehavior(ZobObject* zobObject, TiXmlElement* node);
	static ZobBehavior* CreateBehavior(ZobObject* zobObject, const char* behaviorName);
};