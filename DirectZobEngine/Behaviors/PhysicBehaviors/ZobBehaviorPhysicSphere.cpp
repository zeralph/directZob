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
	Init();
}

void ZobBehaviorPhysicSphere::Init()
{
	m_radius = 1.0f;
	ZobBehaviorPhysicShape::Init();
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	m_sphereShape = pc->createSphereShape(m_radius);
	Transform t;
	t.identity();
	t.setPosition(Vector3(m_localPostion.x, m_localPostion.y, m_localPostion.z));
	AddColliderInternal(m_sphereShape);
	assert(m_collider);
	m_collider->setLocalToBodyTransform(t);
}

void ZobBehaviorPhysicSphere::EditorUpdate()
{
	ZobBehaviorPhysicShape::EditorUpdate();
	if (m_sphereShape->getRadius() != m_radius)
	{
		m_sphereShape->setRadius(m_radius);
	}
}