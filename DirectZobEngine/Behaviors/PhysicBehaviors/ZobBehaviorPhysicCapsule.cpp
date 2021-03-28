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
	WrapVariable(eWrapperType_float, "Radius", &m_radius, false, true);
	WrapVariable(eWrapperType_float, "Height", &m_height, false, true);
	Init();
}


void ZobBehaviorPhysicCapsule::Init()
{
	ZobBehaviorPhysicShape::Init();
	m_radius = 1.0f;
	m_height = 2.0f;
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_capsuleShape = pc->createCapsuleShape(m_radius, m_height);
	AddColliderInternal(m_capsuleShape);
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
