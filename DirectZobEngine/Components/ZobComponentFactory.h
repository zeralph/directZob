#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobComponent.h"


class ZobObject;
class ZobComponentFactory 
{
public:

	static const char* eComponentTypeStr[ZobComponent::__eComponent_MAX__];
	static ZobComponent* CreateComponent(ZobObject* zobObject, TiXmlElement* node);
	static ZobComponent* CreateComponent(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent);

private:
	static ZobComponent* CreateComponentInternal(ZobObject* zobObject, const char* ComponentName, bool bEditorZobComponent);
};