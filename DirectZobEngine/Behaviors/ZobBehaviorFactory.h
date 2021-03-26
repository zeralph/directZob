#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include "ZobBehaviorCar.h"
#include "ZobBehaviorMenu.h"

class ZobObject;
class ZobBehaviorFactory 
{
public:

	static const char* eBehaviorTypeStr[ZobBehavior::__eBehavior_MAX__];
	static ZobBehavior* CreateBehavior(ZobObject* zobObject, TiXmlElement* node);
};