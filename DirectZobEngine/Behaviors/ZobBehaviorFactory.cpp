#include "ZobBehaviorFactory.h"
#include "ZobBehaviorCar.h"
#include "ZobBehaviorMenu.h"
#include "ZobBehaviorMesh.h"
#include "ZobBehaviorSprite.h"
#include "PhysicBehaviors/ZobBehaviorPhysicBox.h"
#include "PhysicBehaviors/ZobBehaviorPhysicSphere.h"
#include "PhysicBehaviors/ZobBehaviorPhysicCapsule.h"
#include "PhysicBehaviors/ZobBehaviorPhysicMesh.h"

const char* ZobBehaviorFactory::eBehaviorTypeStr[ZobBehavior::__eBehavior_MAX__] =
{
	"None",
	"Car",
	"Menu",
	"PhysicBox",
	"PhysicSphere",
	"PhysicCapsule",
	"PhysicMesh",
	"Mesh",
	"Sprite"
};

ZobBehavior* ZobBehaviorFactory::CreateBehaviorInternal(ZobObject* zobObject, const char* behaviorName)
{
	ZobBehavior* zb = NULL;
	if (zobObject)
	{
		if (behaviorName)
		{
			ZobBehavior::eBehaviorType behaviorType = ZobBehavior::eBehavior_none;
			for (int i = 0; i < ZobBehavior::__eBehavior_MAX__; i++)
			{
				if (strcmp(eBehaviorTypeStr[i], behaviorName) == 0)
				{
					behaviorType = (ZobBehavior::eBehaviorType)i;
					break;
				}
			}
			switch (behaviorType)
			{
			case ZobBehavior::eBehavior_car:
				zb = new ZobBehaviorCar(zobObject);
				break;
			case ZobBehavior::eBehavior_physicBox:
				zb = new ZobBehaviorPhysicBox(zobObject);
				break;
			case ZobBehavior::eBehavior_physicSphere:
				zb = new ZobBehaviorPhysicSphere(zobObject);
				break;
			case ZobBehavior::eBehavior_physicCapsule:
				zb = new ZobBehaviorPhysicCapsule(zobObject);
				break;
			case ZobBehavior::eBehavior_physicMesh:
				zb = new ZobBehaviorPhysicMesh(zobObject);
				break;
			case ZobBehavior::eBehavior_menu:
				zb = new ZobBehaviorMenu(zobObject);
				break;
			case ZobBehavior::eBehavior_mesh:
				zb = new ZobBehaviorMesh(zobObject);
				break;
			case ZobBehavior::eBehavior_sprite:
				zb = new ZobBehaviorSprite(zobObject);
				break;
			case ZobBehavior::eBehavior_none:
				zb = NULL;
				break;
			default:
				assert(false);
				break;
			}
		}
		if (zb)
		{
			zobObject->AddBehavior(zb);
		}
	}
	return zb;
}

ZobBehavior* ZobBehaviorFactory::CreateBehavior(ZobObject* zobObject, const char* behaviorName)
{
	ZobBehavior* zb = NULL;
	if (zobObject)
	{
		zb = CreateBehaviorInternal(zobObject, behaviorName);
	}
	return zb;
}

ZobBehavior* ZobBehaviorFactory::CreateBehavior(ZobObject* zobObject, TiXmlElement* node)
{
	ZobBehavior* zb = NULL;
	if (zobObject && node)
	{
		const char* behaviorTypeStr = node->Attribute("Type");
		ZobBehavior* zb = CreateBehaviorInternal(zobObject, behaviorTypeStr);
		if (zb)
		{
			zb->ReadNode(node);
		}
	}
	return zb;
}