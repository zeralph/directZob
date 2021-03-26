#include "ZobBehaviorFactory.h"

const char* ZobBehaviorFactory::eBehaviorTypeStr[ZobBehavior::__eBehavior_MAX__] =
{
	"None",
	"Car",
	"Menu"
};

ZobBehavior* ZobBehaviorFactory::CreateBehavior(ZobObject* zobObject, TiXmlElement* node)
{
	ZobBehavior* zb = NULL;
	if (node)
	{
		const char* behaviorTypeStr = node->Attribute("Type");
		if (behaviorTypeStr)
		{
			ZobBehavior::eBehaviorType behaviorType = ZobBehavior::eBehavior_none;
			for (int i = 0; i < ZobBehavior::__eBehavior_MAX__; i++)
			{
				if (strcmp(eBehaviorTypeStr[i], behaviorTypeStr) == 0)
				{
					behaviorType = (ZobBehavior::eBehaviorType)i;
				}
			}
			switch (behaviorType)
			{
			case ZobBehavior::eBehavior_car:
				zb = new ZobBehaviorCar(zobObject, node);
				break;
			case ZobBehavior::eBehavior_menu:
				zb = new ZobBehaviorMenu(zobObject, node);
				break;
			default:
			case ZobBehavior::eBehavior_none:
				zb = NULL;
				break;
			}
		}
	}
	if (zb)
	{
		zb->LoadVariables(node);
	}
	return zb;
}