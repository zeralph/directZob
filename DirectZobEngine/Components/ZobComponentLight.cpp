#include "ZobComponentLight.h"


ZobComponentLight::ZobComponentLight(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	m_type = eComponent_light;
}

ZobComponentLight::~ZobComponentLight()
{

}

void ZobComponentLight::Init()
{

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
