#include "ZobBehaviorLight.h"


ZobBehaviorLight::ZobBehaviorLight(ZobObject* zobObject, bool bEditorZobBehavior) : ZobBehavior(zobObject, bEditorZobBehavior)
{
	m_type = eBehavior_light;
}

ZobBehaviorLight::~ZobBehaviorLight()
{

}

void ZobBehaviorLight::Init()
{

}

void ZobBehaviorLight::PreUpdate(float dt)
{

}

void ZobBehaviorLight::PostUpdate()
{

}

void ZobBehaviorLight::UpdateBeforeObject(float dt)
{

}

void ZobBehaviorLight::UpdateAfterObject(float dt)
{

}

void ZobBehaviorLight::QueueForDrawing(const Camera* camera, Engine* engine)
{

}

void ZobBehaviorLight::EditorUpdate()
{

}
