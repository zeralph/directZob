#include "ZobComponentFactory.h"
#include "../../dependencies/tinyxml/tinyxml.h"
#include "ZobComponent.h"
#include "ZobComponentCar.h"
#include "HudComponents/ZobComponentText.h"
#include "HudComponents/ZobComponentImage.h"
#include "ZobComponentRotator.h"
#include "GraphicComponents/ZobComponentMesh.h"
#include "GraphicComponents/ZobComponentSprite.h"
#include "GraphicComponents/ZobComponentSkybox.h"
#include "ZobComponentLight.h"
#include "PhysicComponents/ZobComponentPhysicBox.h"
#include "PhysicComponents/ZobComponentPhysicSphere.h"
#include "PhysicComponents/ZobComponentPhysicCapsule.h"
#include "PhysicComponents/ZobComponentPhysicMesh.h"
#include "../DirectZob.h"

const char* ZobComponentFactory::eComponentTypeStr[__eComponent_MAX__] =
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
	"Skybox",
	"Rotator"
};

ZobComponent* ZobComponentFactory::CreateComponentInternal(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent)
{
	ZobComponent* zb = NULL;
	if (zobObject)
	{
		if (ComponentName)
		{
			eComponentType ComponentType = eComponent_none;
			for (int i = 0; i < __eComponent_MAX__; i++)
			{
				if (strcmp(eComponentTypeStr[i], ComponentName) == 0)
				{
					ComponentType = (eComponentType)i;
					break;
				}
			}
			switch (ComponentType)
			{
			case eComponent_car:
				zb = new ZobComponentCar(zobObject);
				break;
			case eComponent_physicBox:
				zb = new ZobComponentPhysicBox(zobObject);
				break;
			case eComponent_physicSphere:
				zb = new ZobComponentPhysicSphere(zobObject);
				break;
			case eComponent_physicCapsule:
				zb = new ZobComponentPhysicCapsule(zobObject);
				break;
			case eComponent_physicMesh:
				zb = new ZobComponentPhysicMesh(zobObject);
				break;
			case eComponent_hudText:	
				//zb = NULL;
				zb = new ZobComponentText(zobObject, bEditorZobComponent);
				break;
			case eComponent_hudImage:
				//zb = NULL;
				zb = new ZobComponentImage(zobObject, bEditorZobComponent);
				break;
			case eComponent_rotator:
				//zb = NULL;
				zb = new ZobComponentRotator(zobObject);
				break;
			case eComponent_mesh:
				zb = new ZobComponentMesh(zobObject, bEditorZobComponent);
				break;
			case eComponent_sprite:
				zb = new ZobComponentSprite(zobObject, bEditorZobComponent);
				break;
			case eComponent_light:
				zb = new ZobComponentLight(zobObject, bEditorZobComponent);
				break;
			case eComponent_none:
				zb = NULL;
				assert(1==0, "component type '%s' of object '%s' not found", ComponentName, zobObject->GetName());
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
