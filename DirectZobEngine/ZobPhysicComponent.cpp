#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"

ZobPhysicComponent::ZobPhysicComponent(TiXmlNode* t)
{
	BodyType rigidBodyType = rp3d::BodyType::STATIC;
	bool rigidBodyActive = false;
	if (t)
	{
		TiXmlElement* p = (TiXmlElement*)t;
		m_type = (ePhysicComponentType)atoi(p->Attribute("type"));
		TiXmlElement* b = p->FirstChildElement("RigidBody");
		BodyType rigidBodyType = (BodyType)atoi(b->Attribute("type"));
		bool rigidBodyActive = (bool)atoi(b->Attribute("active"));
		TiXmlElement* c = p->FirstChildElement("Collider");
	}
	m_type = ePhysicComponentType_none;
	m_rigidBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateRigidBody(&ZobVector3::Vector3Zero, &ZobVector3::Vector3Zero);
	m_rigidBody->setType(rigidBodyType);
	m_rigidBody->setIsActive(rigidBodyActive);
	m_collider = NULL;
}

ZobPhysicComponent::~ZobPhysicComponent()
{
	DirectZob::GetInstance()->GetPhysicsEngine()->DestroyRigidBody(m_rigidBody);
	m_rigidBody = NULL;
	m_type = ePhysicComponentType_none;
}

void ZobPhysicComponent::Init()
{
	if (m_rigidBody)
	{
		m_rigidBody = NULL;
	}

}

TiXmlNode* ZobPhysicComponent::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement p = TiXmlElement("Physic");
	p.SetAttribute("type", std::to_string((int)m_type).c_str());
	TiXmlElement b = TiXmlElement("RigidBody");
	b.SetAttribute("type", std::to_string((int)m_rigidBody->getType()).c_str());
	b.SetAttribute("active", std::to_string((int)m_rigidBody->isActive()).c_str());
	TiXmlElement c = TiXmlElement("Collider");
	c.SetAttribute("type", std::to_string((int)m_shapeDraw._type).c_str());
	c.SetAttribute("radius", std::to_string((float)m_shapeDraw._radius).c_str());
	c.SetAttribute("height", std::to_string((float)m_shapeDraw._height).c_str());
	c.SetAttribute("halfExtends_x", std::to_string((float)m_shapeDraw._halfExtends.x).c_str());
	c.SetAttribute("halfExtends_y", std::to_string((float)m_shapeDraw._halfExtends.y).c_str());
	c.SetAttribute("halfExtends_z", std::to_string((float)m_shapeDraw._halfExtends.z).c_str());
	p.InsertEndChild(b);
	p.InsertEndChild(c);
	node->InsertEndChild(p);
	return NULL;
}

void ZobPhysicComponent::Set(ePhysicComponentType t)
{
	ZobVector3 z = ZobVector3(4, 4, 4);
	if (m_collider)
	{
		m_rigidBody->removeCollider(m_collider);
		delete m_collider;
		m_collider = NULL;
	}
	m_type = t;
	switch (m_type)
	{
	case ePhysicComponentType_none:
		m_rigidBody->setType(rp3d::BodyType::STATIC);
		m_rigidBody->setIsActive(false);
		break;
	case ePhysicComponentType_static:
		m_rigidBody->setType(rp3d::BodyType::STATIC);
		m_rigidBody->setIsActive(true);
		AddBoxCollider(&z);
		break;
	case ePhysicComponentType_dynamic:
		m_rigidBody->setType(rp3d::BodyType::DYNAMIC);
		m_rigidBody->setIsActive(true);
		AddSphereCollider(2.0f);
		break;
	}
}

void ZobPhysicComponent::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	Vector3 v = Vector3(x, y, z);
	Transform t = Transform(m_rigidBody->getTransform());
	t.setPosition(v);
	m_rigidBody->setTransform(t);
}

void ZobPhysicComponent::SetOrientation(float x, float y, float z)
{
	m_orientation.x = x;
	m_orientation.y = y;
	m_orientation.z = z;
	Transform t = Transform(m_rigidBody->getTransform());
	Quaternion q = Quaternion::fromEulerAngles(x * 180.0f / M_PI, y * 180.0f / M_PI, z * 180.0f / M_PI);
	t.setOrientation(q);
	m_rigidBody->setTransform(t);
}

const ZobVector3* ZobPhysicComponent::GetPosition()
{
	if (m_type == ePhysicComponentType_dynamic)
	{
		const Vector3 v = m_rigidBody->getTransform().getPosition();
		m_position = ZobVector3(v.x, v.y, v.z);
	}
	return &m_position;
}

const ZobVector3* ZobPhysicComponent::GetOrientation()
{
	if (m_type == ePhysicComponentType_dynamic)
	{
		const Quaternion v = m_rigidBody->getTransform().getOrientation();
		m_orientation = ZobVector3(v.x *  M_PI / 180.0f, v.y * M_PI / 180.0f, v.z * M_PI / 180.0f);
	}
	return &m_orientation;
}

void ZobPhysicComponent::AddBoxCollider(const ZobVector3* halfExtends)
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(halfExtends->x, halfExtends->y, halfExtends->z);
	BoxShape* s = pc->createBoxShape(h);
	m_shapeDraw._type = sShapeDraw::eShapeType::eShapeType_box;
	m_shapeDraw._halfExtends = halfExtends;
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddSphereCollider(float radius)
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	SphereShape* s= pc->createSphereShape(radius);
	m_shapeDraw._type = sShapeDraw::eShapeType::eShapeType_sphere;
	m_shapeDraw._radius = radius;
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

	m_rigidBody->addCollider(c, m_rigidBody->getTransform());

}

void ZobPhysicComponent::SaveTransform()
{

	m_savedTransform = m_rigidBody->getTransform();

}

void ZobPhysicComponent::RestoreTransform()
{
	m_rigidBody->setTransform(m_savedTransform);
}

void ZobPhysicComponent::ResetPhysic()
{
	if(m_rigidBody->isActive())
	{
		m_rigidBody->setIsActive(false);
		m_rigidBody->setIsActive(true);
	}
}

void ZobPhysicComponent::DrawGizmos(const Camera* camera, const ZobMatrix4x4* mat)
{
	uint c = 0x00FF00;
	bool bold = false;
	Engine* e = DirectZob::GetInstance()->GetEngine();
	switch (m_shapeDraw._type)
	{
	case sShapeDraw::eShapeType::eShapeType_sphere:
		e->QueueSphere(camera, mat, m_shapeDraw._radius, c, bold);
		break;
	case sShapeDraw::eShapeType::eShapeType_box:
		e->QueueBox(camera, mat, m_shapeDraw._halfExtends, c, bold);
		break;
	case sShapeDraw::eShapeType::eShapeType_capsule:
	case sShapeDraw::eShapeType::eShapeType_convexMesh:
	default:
		break;
	}
}