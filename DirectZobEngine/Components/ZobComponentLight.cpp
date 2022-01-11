#include "ZobComponentLight.h"
#include "../ZobObjects/ZobObject.h"
#include "ZobComponentFactory.h"

ZobComponentLight::ZobComponentLight(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	m_componentType = ZobComponentFactory::eComponent_light;
	SET_CLASS_AND_NAME
}

ZobComponentLight::~ZobComponentLight()
{

}

void ZobComponentLight::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (cb)
	{
		cb(0, 0, m_name);
	}
}

void ZobComponentLight::PreUpdate(float dt)
{

}

void ZobComponentLight::PostUpdate()
{

}

void ZobComponentLight::UpdateBeforeObject(float dt)
{

}

void ZobComponentLight::UpdateAfterObject(float dt)
{

}

void ZobComponentLight::QueueForDrawing(const Camera* camera, Engine* engine)
{

}

void ZobComponentLight::EditorUpdate()
{

}
