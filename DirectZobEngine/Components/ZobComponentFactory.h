#pragma once
#include "../Types.h"

class ZobComponent;
class ZobObject;
class TiXmlElement;
class ZobComponentFactory 
{
public:
	enum eComponentType
	{
		eComponent_none = 0,
		eComponent_car,
		eComponent_hudText,
		eComponent_hudImage,
		eComponent_physicBox,
		eComponent_physicSphere,
		eComponent_physicCapsule,
		eComponent_physicMesh,
		eComponent_mesh,
		eComponent_sprite,
		eComponent_light,
		eComponent_skybox,
		__eComponent_MAX__,
	};
	static const char* eComponentTypeStr[__eComponent_MAX__];
	static ZobComponent* CreateComponent(ZobObject* zobObject, TiXmlElement* node);
	static ZobComponent* CreateComponent(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent);

private:
	static ZobComponent* CreateComponentInternal(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent);
};