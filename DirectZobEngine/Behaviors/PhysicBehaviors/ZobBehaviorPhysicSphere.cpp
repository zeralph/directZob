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
	m_localPostion = ZobVector3(0,0,0);
	WrapVariable(eWrapperType_float, "Radius", &m_radius, false, true);
	m_radius = 1.0f;
}

void ZobBehaviorPhysicSphere::Init()
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_sphereShape = pc->createSphereShape(m_radius);
	AddColliderInternal(m_sphereShape);
	assert(m_collider);
	ZobBehaviorPhysicShape::Init();
}

void ZobBehaviorPhysicSphere::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	if (m_sphereShape->getRadius() != m_radius)
	{
		m_sphereShape->setRadius(m_radius);
	}
}