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

	static ZobBehavior* CreateBehavior(ZobObject* zobObject, TiXmlElement* node)
	{
		if (node)
		{
			std::string behaviorTypeStr = node->Attribute("type");
			if (!behaviorTypeStr.empty())
			{
				ZobBehavior::eBehaviorType behaviorType = (ZobBehavior::eBehaviorType)atoi(behaviorTypeStr.c_str());
				switch (behaviorType)
				{
				case ZobBehavior::eBehavior_car:
					return new ZobBehaviorCar(zobObject, node);
				case ZobBehavior::eBehavior_menu:
					return new ZobBehaviorMenu(zobObject, node);
				default:
				case ZobBehavior::eBehavior_none:
					return NULL;
				}
			}
		}
		return NULL;
	}
};