#include "ZobBehaviorPhysicSphere.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicSphere::~ZobBehaviorPhysicSphere()
{

}

ZobBehaviorPhysicSphere::ZobBehaviorPhysicSphere(ZobObject* zobObject) : ZobBehaviorPhysicShape(zobObject)
{
	m_type = eBehavior_physicSphere;
	m_sphereShape = NULL;
	WrapVariable(eWrapperType_float, "Radius", &m_radius, false, true);
	Init();
}

void ZobBehaviorPhysicSphere::Init()
{
	m_radius = 1.0f;
	ZobBehaviorPhysicShape::Init();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_sphereShape = pc->createSphereShape(m_radius);
	//m_collider->setLocalToBodyTransform
	AddColliderInternal(m_sphereShape);
}

void ZobBehaviorPhysicSphere::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	if (m_sphereShape->getRadius() != m_radius)
	{
		m_sphereShape->setRadius(m_radius);
	}
}