#include "ZobBehaviorPhysicCapsule.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicCapsule::~ZobBehaviorPhysicCapsule()
{
	
}

ZobBehaviorPhysicCapsule::ZobBehaviorPhysicCapsule(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicCapsule;
	m_capsuleShape = NULL;
	m_varExposer->WrapVariable<float>("Radius", &m_radius, false, true);
	m_varExposer->WrapVariable<float>("Height", &m_height, false, true);
	m_radius = 1.0f;
	m_height = 2.0f;
}


void ZobBehaviorPhysicCapsule::Init()
{
	ReLoadVariables();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_capsuleShape = pc->createCapsuleShape(m_radius, m_height);
	AddColliderInternal(m_capsuleShape);
	ZobBehaviorPhysicShape::Init();
}

void ZobBehaviorPhysicCapsule::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	if (m_capsuleShape->getRadius() != m_radius)
	{
		m_capsuleShape->setRadius(m_radius);
	}
	if (m_capsuleShape->getHeight() != m_height)
	{
		m_capsuleShape->setHeight(m_height);
	}
}
