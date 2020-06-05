#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"

ZobPhysicComponent::ZobPhysicComponent():
m_type(ePhysicComponentType_none),
m_collisionBody(NULL),
m_rigidBody(NULL),
m_position(0,0,0),
m_orientation(0,0,0)
{

}

ZobPhysicComponent::~ZobPhysicComponent()
{
	UnSet();
}

void ZobPhysicComponent::UnSet()
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		break;
	case ePhysicComponentType_collisionBody:
	{
		DirectZob::GetInstance()->GetPhysicsEngine()->DestroyCollisionBody(m_collisionBody);
		m_collisionBody = NULL;
		break;
	}
	case ePhysicComponentType_rigidBody:
		DirectZob::GetInstance()->GetPhysicsEngine()->DestroyRigidBody(m_rigidBody);
		m_rigidBody = NULL;
		break;
	};
	m_type = ePhysicComponentType_none;
}

void ZobPhysicComponent::Set(ePhysicComponentType t)
{
	UnSet();
	m_type = t;
	switch (m_type)
	{
	case ePhysicComponentType_none:
		break;
	case ePhysicComponentType_collisionBody:
	{
		m_collisionBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateCollisionBody(&m_position, &m_orientation);
		break;
	}
	case ePhysicComponentType_rigidBody:
		m_rigidBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateRigidBody(&m_position, &m_orientation);
		break;
	};
}

void ZobPhysicComponent::SetPosition(float x, float y, float z)
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		m_position = ZobVector3(x, y, z);
		break;
	case ePhysicComponentType_collisionBody:
	{
		Transform t = Transform(m_collisionBody->getTransform());
		Vector3 v = Vector3(x, y, z);
		t.setPosition(v);
		m_collisionBody->setTransform(t);
		break;
	}
	case ePhysicComponentType_rigidBody:
		Transform t = Transform(m_collisionBody->getTransform());
		Vector3 v = Vector3(x, y, z);
		t.setPosition(v);
		m_rigidBody->setTransform(t);
		break;
	}
}

void ZobPhysicComponent::SetOrientation(float x, float y, float z)
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		m_orientation = ZobVector3(x, y, z);
		break;
	case ePhysicComponentType_collisionBody:
	{
		Transform t = Transform(m_collisionBody->getTransform());
		Quaternion q = Quaternion::identity();
		q.fromEulerAngles(x, y, z);
		t.setOrientation(q);
		m_collisionBody->setTransform(t);
		break;
	}
	case ePhysicComponentType_rigidBody:
		Transform t = Transform(m_collisionBody->getTransform());
		Quaternion q = Quaternion::identity();
		q.fromEulerAngles(x, y, z);
		t.setOrientation(q);
		m_rigidBody->setTransform(t);
		break;
	}
}

const ZobVector3* ZobPhysicComponent::GetPosition()
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		&m_position;
		break;
	case ePhysicComponentType_collisionBody:
	{
		const Vector3 v = m_collisionBody->getTransform().getPosition();
		m_position.x = v.x;
		m_position.y = v.y;
		m_position.z = v.z;
		break;
	}
	case ePhysicComponentType_rigidBody:
		const Vector3 v = m_rigidBody->getTransform().getPosition();
		m_position.x = v.x;
		m_position.y = v.y;
		m_position.z = v.z;
		break;
	}
	return &m_position;
}

const ZobVector3* ZobPhysicComponent::GetOrientation()
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		break;
	case ePhysicComponentType_collisionBody:
	{
		const Quaternion q = m_collisionBody->getTransform().getOrientation();
		m_orientation.x = q.x;
		m_orientation.y = q.y;
		m_orientation.z = q.z;
		break;
	}
	case ePhysicComponentType_rigidBody:
		const Quaternion q = m_rigidBody->getTransform().getOrientation();
		m_orientation.x = q.x;
		m_orientation.y = q.y;
		m_orientation.z = q.z;
		break;
	}
	return &m_orientation;
}

void ZobPhysicComponent::AddBoxCollider(const ZobVector3* halfExtends)
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(halfExtends->x, halfExtends->y, halfExtends->z);
	BoxShape* boxShape = pc->createBoxShape(h);
	AddColliderInternal(boxShape);
}

void ZobPhysicComponent::AddSphereCollider(float radius)
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	SphereShape* s= pc->createSphereShape(radius);
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddCapsuleCollider(float radius, float height)
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	CapsuleShape* s = pc->createCapsuleShape(radius, height);
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddColliderInternal(CollisionShape* c)
{
	switch (m_type)
	{
	case ePhysicComponentType_none:
		DirectZob::GetInstance()->LogWarning("Trying to add a collider to a physic object with no physic component");
		break;
	case ePhysicComponentType_collisionBody:
	{
		m_collisionBody->addCollider(c, m_collisionBody->getTransform());
		break;
	}
	case ePhysicComponentType_rigidBody:
		m_rigidBody->addCollider(c, m_rigidBody->getTransform());
		break;
	}
}