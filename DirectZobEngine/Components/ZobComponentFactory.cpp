#include "ZobComponentFactory.h"
#include "ZobComponentCar.h"
#include "HudComponents/ZobComponentText.h"
#include "HudComponents/ZobComponentImage.h"
#include "GraphicComponents/ZobComponentMesh.h"
#include "GraphicComponents/ZobComponentSprite.h"
#include "GraphicComponents/ZobComponentSkybox.h"
#include "ZobComponentLight.h"
#include "PhysicComponents/ZobComponentPhysicBox.h"
#include "PhysicComponents/ZobComponentPhysicSphere.h"
#include "PhysicComponents/ZobComponentPhysicCapsule.h"
#include "PhysicComponents/ZobComponentPhysicMesh.h"
#include "../DirectZob.h"

const char* ZobComponentFactory::eComponentTypeStr[ZobComponent::__eComponent_MAX__] =
{
	"None",
	"Car",
	"Text",
	"Image",
	"PhysicBox",
	"PhysicSphere",
	"PhysicCapsule",
	"PhysicMesh",
	"Mesh",
	"Sprite",
	"Light",
	"Skybox"
};

ZobComponent* ZobComponentFactory::CreateComponentInternal(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent)
{
	ZobComponent* zb = NULL;
	if (zobObject)
	{
		if (ComponentName)
		{
			ZobComponent::eComponentType ComponentType = ZobComponent::eComponent_none;
			for (int i = 0; i < ZobComponent::__eComponent_MAX__; i++)
			{
				if (strcmp(eComponentTypeStr[i], ComponentName) == 0)
				{
					ComponentType = (ZobComponent::eComponentType)i;
					break;
				}
			}
			switch (ComponentType)
			{
			case ZobComponent::eComponent_car:
				zb = new ZobComponentCar(zobObject);
				break;
			case ZobComponent::eComponent_physicBox:
				zb = new ZobComponentPhysicBox(zobObject);
				break;
			case ZobComponent::eComponent_physicSphere:
				zb = new ZobComponentPhysicSphere(zobObject);
				break;
			case ZobComponent::eComponent_physicCapsule:
				zb = new ZobComponentPhysicCapsule(zobObject);
				break;
			case ZobComponent::eComponent_physicMesh:
				zb = new ZobComponentPhysicMesh(zobObject);
				break;
			case ZobComponent::eComponent_hudText:
				zb = new ZobComponentText(zobObject, bEditorZobComponent);
				break;
			case ZobComponent::eComponent_hudImage:
				zb = new ZobComponentImage(zobObject, bEditorZobComponent);
				break;
			case ZobComponent::eComponent_mesh:
				zb = new ZobComponentMesh(zobObject, bEditorZobComponent);
				break;
			case ZobComponent::eComponent_sprite:
				zb = new ZobComponentSprite(zobObject, bEditorZobComponent);
				break;
			case ZobComponent::eComponent_light:
				zb = new ZobComponentLight(zobObject, bEditorZobComponent);
				break;
			case ZobComponent::eComponent_none:
				zb = NULL;
				break;
			default:
				assert(false);
				break;
			}
		}
		if (zb)
		{
			zobObject->AddComponent(zb);
		}
	}
	return zb;
}

ZobComponent* ZobComponentFactory::CreateComponent(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent)
{
	ZobComponent* zb = NULL;
	if (zobObject)
	{
		zb = CreateComponentInternal(zobObject, ComponentName, bEditorZobComponent);
	}
	return zb;
}

ZobComponent* ZobComponentFactory::CreateComponent(ZobObject* zobObject, TiXmlElement* node)
{
	ZobComponent* zb = NULL;
	if (zobObject && node)
	{
		const char* ComponentTypeStr = node->Attribute("Type");
		ZobComponent* zb = CreateComponentInternal(zobObject, ComponentTypeStr, false);
		if (zb)
		{
			zb->ReadNode(node);
			//zb->Init(); NOT HERE
		}
	}
	return zb;
}
