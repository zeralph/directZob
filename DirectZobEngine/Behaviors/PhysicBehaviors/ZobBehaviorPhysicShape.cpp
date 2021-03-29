#include "ZobBehaviorPhysicShape.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobBehaviorPhysicShape::~ZobBehaviorPhysicShape()
{

}

ZobBehaviorPhysicShape::ZobBehaviorPhysicShape(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_type = eBehavior_none;


	int pl[] = { ZobPhysicComponent::eLayer_none, ZobPhysicComponent::eLayer_ground, ZobPhysicComponent::eLayer_wall, ZobPhysicComponent::eLayer_objects };
	const char* sl[] = { "None", "Ground", "Wall", "Objects" };
	WrapEnum("Layer", &m_layer, 4, pl, sl, false, true);

	WrapVariable(eWrapperType_string, "test", &m_test, false, true);
	WrapVariable(eWrapperType_ZobVector3, "Local position", &m_localPostion, false, true);
	WrapVariable(eWrapperType_float, "Bounciness",  &m_bounciness, false, true);
	WrapVariable(eWrapperType_float, "Friction coeff", &m_frictionCoeff, false, true);
	WrapVariable(eWrapperType_float, "Mass density", &m_massDensity, false, true);
	WrapVariable(eWrapperType_float, "Rollling Resistance", &m_rollingResistance, false, true);
	WrapVariable(eWrapperType_bool, "Scale with object", &m_bUpdateSize, false, true);

	Init();
}

void ZobBehaviorPhysicShape::PreUpdate()
{

}

void ZobBehaviorPhysicShape::Init()
{
	m_collider = NULL;
	m_rollingResistance = 1.0f;
	m_massDensity = 1.0f;
	m_frictionCoeff = 1.0f;
	m_bounciness = 1.0f;
	m_layer = ZobPhysicComponent::eLayer_none;
}

void ZobBehaviorPhysicShape::EditorUpdate()
{
	if (m_collider)
	{
		reactphysics3d::Material& material = m_collider->getMaterial();
		if (material.getBounciness() != m_bounciness)
		{
			material.setBounciness(m_bounciness);
		}

		if (material.getFrictionCoefficient() != m_frictionCoeff)
		{
			material.setFrictionCoefficient(m_frictionCoeff);
		}
		if (material.getMassDensity() != m_massDensity)
		{
			material.setMassDensity(m_massDensity);
		}
		if (material.getRollingResistance() != m_rollingResistance)
		{
			material.setRollingResistance(m_rollingResistance);
		}
		Vector3 v = m_collider->getLocalToBodyTransform().getPosition();
		if (m_localPostion.x != v.x || m_localPostion.z != v.z || m_localPostion.z != v.z)
		{
			Transform t = m_collider->getLocalToBodyTransform();
			v.x = m_localPostion.x;
			v.y = m_localPostion.y;
			v.z = m_localPostion.z;
			t.setPosition(v);
			m_collider->setLocalToBodyTransform(t);
		}
	}
}

void ZobBehaviorPhysicShape::Update(float dt)
{

}

void ZobBehaviorPhysicShape::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{
}

void ZobBehaviorPhysicShape::AddColliderInternal(CollisionShape* c)
{
	assert(m_collider == NULL);
	Transform t = Transform::identity();
	m_collider = m_zobObject->GetPhysicComponentNoConst()->GetRigidBody()->addCollider(c, t);
}

void ZobBehaviorPhysicShape::RemoveCollider()
{
	if (m_collider)
	{
		m_zobObject->GetPhysicComponentNoConst()->GetRigidBody()->removeCollider(m_collider);
		m_collider = NULL;
	}
}
