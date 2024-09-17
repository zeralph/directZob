#include "ZobComponentRotator.h"
#include "../ZobObjects/ZobObject.h"
#include "ZobComponentFactory.h"
#include "../Rendering/Engine.h"
#include "../ZobObjects/ZobObject.h"

ZobComponentRotator::ZobComponentRotator(ZobObject* zobObject) : ZobComponent(zobObject, false)
{
	m_rotateXdt = 0.0f;
	m_rotateYdt = 0.0f;
	m_rotateZdt = 0.0f;
	m_componentType = ZobComponentFactory::eComponent_rotator;
	SET_CLASS_AND_NAME
	Init(NULL);
	m_varExposer->WrapVariable<float>("Rotate X", &m_rotateXdt, NULL, false, true);
	m_varExposer->WrapVariable<float>("Rotate Y", &m_rotateYdt, NULL, false, true);
	m_varExposer->WrapVariable<float>("Rotate Z", &m_rotateZdt, NULL, false, true);
}

ZobComponentRotator::~ZobComponentRotator()
{

}

void ZobComponentRotator::Init(DirectZobType::sceneLoadingCallback cb)
{
	ReLoadVariables();
}

void ZobComponentRotator::PreUpdate(float dt)
{
	m_zobObject->SetLocalRotation(m_rotateXdt, m_rotateYdt, m_rotateZdt, true);
}

void ZobComponentRotator::PostUpdate()
{

}

void ZobComponentRotator::QueueForDrawing(const Camera* camera, Engine* engine)
{

}

void ZobComponentRotator::EditorUpdate()
{

}
