#include "ZobComponentPhysicShape.h"
#include "Types.h"
#include "../../DirectZob.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"

ZobComponentPhysicShape::~ZobComponentPhysicShape()
{
	RemoveCollider();
}

ZobComponentPhysicShape::ZobComponentPhysicShape(ZobObject* zobObject) : ZobComponent(zobObject, false)
{
	SET_CLASS_AND_NAME
	m_componentType = eComponent_none;
	m_isInit = false;
	m_varExposer->WrapVariable<int>("Layers", &m_layers, &ZobComponentPhysicShape::OnVariablesChanged, false, true);
	m_varExposer->WrapVariable<bool>("Trigger", &m_isTrigger, ZobComponentPhysicShape::OnVariablesChanged, false, true);
	m_varExposer->WrapVariable<ZobVector3>("Local position", &m_localPostion, ZobComponentPhysicShape::OnVariablesChanged, false, true);
	m_varExposer->WrapVariable<bool>("Scale with object", &m_bUpdateSize, ZobComponentPhysicShape::OnVariablesChanged, false, true);
	m_isTrigger = false;
	m_collider = NULL;
	m_layers = 0;
}

void ZobComponentPhysicShape::PreUpdate(float dt)
{

}

void ZobComponentPhysicShape::PostUpdate()
{

}

void ZobComponentPhysicShape::OnVariablesChanged(zobId id)
{
	ZobComponentPhysicShape* zc = ZobEntity::GetEntity< ZobComponentPhysicShape>(id);
	if(zc)
	{
		assert(zc->m_collider);
		zc->m_collider->setCollisionCategoryBits((short)zc->m_layers);
		zc->m_collider->setIsTrigger(zc->m_isTrigger);
		Vector3 v = zc->m_collider->getLocalToBodyTransform().getPosition();
		Transform t = zc->m_collider->getLocalToBodyTransform();
		v.x = zc->m_localPostion.x;
		v.y = zc->m_localPostion.y;
		v.z = zc->m_localPostion.z;
		t.setPosition(v);
		zc->m_collider->setLocalToBodyTransform(t);
	}
}

void ZobComponentPhysicShape::Init(DirectZobType::sceneLoadingCallback cb)
{
	if(cb)
	{
		cb(0, 0, m_name);
	}
	assert(m_collider);
	//reactphysics3d::Material& material = m_collider->getMaterial();
	//material.setBounciness(m_bounciness);
	//material.setFrictionCoefficient(m_frictionCoeff);
	//material.setMassDensity(m_massDensity);
	//material.setRollingResistance(m_rollingResistance);
	m_collider->setCollisionCategoryBits((short)m_layers);
	//m_collider->setCollideWithMaskBits(0xFFFF);
	m_collider->setIsTrigger(false);
	Vector3 v = m_collider->getLocalToBodyTransform().getPosition();
	Transform t = m_collider->getLocalToBodyTransform();
	v.x = m_localPostion.x;
	v.y = m_localPostion.y;
	v.z = m_localPostion.z;
	t.setPosition(v);
	m_collider->setLocalToBodyTransform(t);
	m_isInit = true;
}

void ZobComponentPhysicShape::EditorUpdate()
{
	if (m_isInit && m_collider)
	{
		reactphysics3d::Material& material = m_collider->getMaterial();
		//if (material.getBounciness() != m_bounciness)
		//{
		//	material.setBounciness(m_bounciness);
		//}

		//if (material.getFrictionCoefficient() != m_frictionCoeff)
		//{
		//	material.setFrictionCoefficient(m_frictionCoeff);
		//}
		//if (material.getMassDensity() != m_massDensity)
		//{
		//	material.setMassDensity(m_massDensity);
		//}
		//if (material.getRollingResistance() != m_rollingResistance)
		//{
		//	material.setRollingResistance(m_rollingResistance);
		//}
		if (m_collider->getCollisionCategoryBits() != m_layers)
		{
			//m_collider->setCollisionCategoryBits(m_layers);
			//m_collider->setCollideWithMaskBits(0xFFFF);
		}
		if (m_collider->getIsTrigger() != m_isTrigger)
		{
			m_collider->setIsTrigger(false);
		}
		Vector3 v = m_collider->getLocalToBodyTransform().getPosition();
		if (m_localPostion.x != v.x || m_localPostion.z != v.z || m_localPostion.z != v.z)
		{
			Transform t = m_collider->getLocalToBodyTransform();
			v.x = m_localPostion.x;
			v.y = m_localPostion.y;
			v.z = m_localPostion.z;
			t.setPosition(v);
			if (t.isValid())
			{
				//crash ici, no se !
				m_collider->setLocalToBodyTransform(t);
			}
		}
	}
}

void ZobComponentPhysicShape::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{
}

void ZobComponentPhysicShape::AddColliderInternal(CollisionShape* c)
{
	assert(m_collider == NULL);
	Transform t = Transform::identity();
	m_collider = m_zobObject->GetPhysicComponentNoConst()->GetRigidBody()->addCollider(c, t);
}

void ZobComponentPhysicShape::RemoveCollider()
{
	if (m_collider)
	{
		m_zobObject->GetPhysicComponentNoConst()->GetRigidBody()->removeCollider(m_collider);
		m_collider = NULL;
	}
}
