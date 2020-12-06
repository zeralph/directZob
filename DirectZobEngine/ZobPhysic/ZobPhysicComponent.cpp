#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"
#include "Rendering/ZobMatrix4x4.h"


ZobPhysicComponent::ZobPhysicComponent(TiXmlNode* t)
{
	BodyType rigidBodyType = rp3d::BodyType::STATIC;
	m_radius = 1.0f;
	m_height = 2.0f;
	m_halfExtends = ZobVector3(1, 1, 1);
	bool rigidBodyActive = false;
	m_type = ePhysicComponentType_none;
	m_shapeType = eShapeType_none;
	m_nextShapeType = eShapeType_none;
	m_collider = NULL;
	if (t)
	{
		TiXmlElement* p = (TiXmlElement*)t;
		m_type = (ePhysicComponentType)atoi(p->Attribute("type"));
		TiXmlElement* b = p->FirstChildElement("RigidBody");
		rigidBodyType = (BodyType)atoi(b->Attribute("type"));
		rigidBodyActive = atoi(b->Attribute("active")) == 1 ? true : false;
		TiXmlElement* c = p->FirstChildElement("Collider");
		m_nextShapeType = (eShapeType)atoi(c->Attribute("type"));
		m_radius = (float)atof(c->Attribute("radius"));
		m_height = (float)atof(c->Attribute("height"));
		m_halfExtends.x = (float)atof(c->Attribute("halfExtends_x"));
		m_halfExtends.y = (float)atof(c->Attribute("halfExtends_y"));
		m_halfExtends.z = (float)atof(c->Attribute("halfExtends_z"));
	}
	m_rigidBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateRigidBody(&ZobVector3::Vector3Zero, &ZobVector3::Vector3Zero);
	m_rigidBody->setType(rigidBodyType);
	UpdateShapeType();
	m_rigidBody->setIsActive(rigidBodyActive);
	
}

ZobPhysicComponent::~ZobPhysicComponent()
{
	DirectZob::GetInstance()->GetPhysicsEngine()->DestroyRigidBody(m_rigidBody);
	m_rigidBody = NULL;
	m_type = ePhysicComponentType_none;
}

void ZobPhysicComponent::Init(const ZobVector3* position, const ZobVector3* rotation)
{
	m_localTransform = Transform::identity();
	m_worldTransform = Transform::identity();
	m_localTransform.setPosition(Vector3(position->x, position->y, position->z));
	Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(rotation->x), DEG_TO_RAD(rotation->y), DEG_TO_RAD(rotation->z));
	m_localTransform.setOrientation(q);
	m_scale = Vector3(1, 1, 1);
	m_totalScale = Vector3(1, 1, 1);
	Update();
}

TiXmlNode* ZobPhysicComponent::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement p = TiXmlElement("Physic");
	p.SetAttribute("type", std::to_string((int)m_type).c_str());
	TiXmlElement b = TiXmlElement("RigidBody");
	b.SetAttribute("type", std::to_string((int)m_rigidBody->getType()).c_str());
	b.SetAttribute("active", std::to_string((int)m_rigidBody->isActive()).c_str());
	TiXmlElement c = TiXmlElement("Collider");
	c.SetAttribute("type", std::to_string((int)m_shapeType).c_str());
	c.SetAttribute("radius", std::to_string((float)m_radius).c_str());
	c.SetAttribute("height", std::to_string((float)m_height).c_str());
	c.SetAttribute("halfExtends_x", std::to_string((float)m_halfExtends.x).c_str());
	c.SetAttribute("halfExtends_y", std::to_string((float)m_halfExtends.y).c_str());
	c.SetAttribute("halfExtends_z", std::to_string((float)m_halfExtends.z).c_str());
	p.InsertEndChild(b);
	p.InsertEndChild(c);
	node->InsertEndChild(p);
	return NULL;
}

void ZobPhysicComponent::SetType(ePhysicComponentType t)
{
	if (m_collider)
	{
		m_rigidBody->removeCollider(m_collider);
		m_collider = NULL;
	}
	m_type = t;
	switch (m_type)
	{
	case ePhysicComponentType_none:
		m_rigidBody->setType(rp3d::BodyType::STATIC);
		m_rigidBody->setIsActive(false);
		m_nextShapeType = eShapeType_none;
		break;
	case ePhysicComponentType_static:
		m_rigidBody->setType(rp3d::BodyType::STATIC);
		m_rigidBody->setIsActive(true);
		AddBoxCollider();
		break;
	case ePhysicComponentType_dynamic:
		m_rigidBody->setType(rp3d::BodyType::DYNAMIC);
		m_rigidBody->setIsActive(true);
		AddSphereCollider();
		break;
	}
}

void ZobPhysicComponent::SetShapeType(eShapeType t)
{
	if (m_type != ePhysicComponentType_none)
	{
		m_nextShapeType = t;
	}
	else
	{
		m_nextShapeType = eShapeType_none;
	}
}

void ZobPhysicComponent::UpdateShapeType()
{
	if(m_nextShapeType != m_shapeType)
	{ 
		m_shapeType = m_nextShapeType;
		switch(m_shapeType)
		{
		case eShapeType_box:
			AddBoxCollider();
			break;
		case eShapeType_capsule:
			AddCapsuleCollider();
			break;
		case eShapeType_convexMesh:
		
			break;
		case eShapeType_sphere:
			AddSphereCollider();
			break;
		default:
			if (m_collider)
			{
				m_rigidBody->removeCollider(m_collider);
				m_collider = NULL;
			}
			break;
		}
	}
}


ZobVector3 ZobPhysicComponent::GetPosition() const
{
	const Vector3 v = GetWorldTransform().getPosition();
	return ZobVector3(v.x, v.y, v.z);
}

void ZobPhysicComponent::SetPosition(float x, float y, float z)
{
	Vector3 v = Vector3(x, y, z);
	Transform t = Transform(m_rigidBody->getTransform());
	t.setPosition(v);
	m_rigidBody->setTransform(t);
}

void ZobPhysicComponent::SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw)
{
	reactphysics3d::Matrix3x3 m;
	m.setAllValues(left->x, up->x, fw->x,
		left->y, up->y, fw->y,
		left->z, up->z, fw->z);
	Quaternion q = Quaternion(m);
	m_worldTransform.setOrientation(q);
}

void ZobPhysicComponent::SetQuaternion(float x, float y, float z, float w)
{
	Quaternion q = Quaternion(x, y, z, w);
	m_worldTransform.setOrientation(q);
}
void ZobPhysicComponent::LookAt(const ZobVector3* target)
{
	Transform t = Transform(m_worldTransform);
	ZobVector3 v = ZobVector3(t.getPosition().x, t.getPosition().y, t.getPosition().z);
	v.x = target->x - v.x;
	v.y = target->y - v.y;
	v.z = target->z - v.z;
	v.Normalize();
	if (v.y > 0.99f)
	{
		int g = 0;
		g++;
	}
	//TODO if v.y > 0.99 ...
	ZobVector3 forward = v;
	ZobVector3 left = ZobVector3::Cross(&forward, &ZobVector3::Vector3Y);
	left.Normalize();
	left.Mul(-1.0f);
	ZobVector3 up = ZobVector3::Cross(&forward, &left);
	up.Normalize();
	LookAt(&forward, &left, &up);
}

void ZobPhysicComponent::LookAt(const ZobVector3* forward, const ZobVector3* left, const ZobVector3* up)
{
	Transform t = Transform(m_worldTransform);
	Quaternion q;
	Quaternion q2 = t.getOrientation();

	q.w = sqrt(fmaxf(0, 1 + left->x + up->y + forward->z)) / 2.0f;
	q.x = sqrt(fmaxf(0, 1 + left->x - up->y - forward->z)) / 2.0f;
	q.y = sqrt(fmaxf(0, 1 - left->x + up->y - forward->z)) / 2.0f;
	q.z = sqrt(fmaxf(0, 1 - left->x - up->y + forward->z)) / 2.0f;
	
	q.x = copysignf(q.x, up->z - forward->y);
	q.y = copysignf(q.y, forward->x - left->z);
	q.z = copysignf(q.z, left->y - up->x);
	
	//q.normalize();
	if (q.isValid())
	{
		t.setOrientation(q);
		m_rigidBody->setTransform(t);
	}
}

void ZobPhysicComponent::SetOrientation(float x, float y, float z)
{
	float rx = DEG_TO_RAD(x);
	float ry = DEG_TO_RAD(y);
	float rz = DEG_TO_RAD(z);
	Transform t = Transform(m_worldTransform);
	Quaternion q;
	//q = Quaternion::fromEulerAngles((dx), (dy), (dz));
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(rx, ry, rz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	Quaternion q2 = t.getOrientation();
	q = q2 * q;
	t.setOrientation(q);
	m_rigidBody->setTransform(t);
}

ZobVector3 ZobPhysicComponent::GetOrientation() const
{
	Quaternion q = GetWorldTransform().getOrientation();
	q.normalize();
	ZobVector3 z = ZobMatrix4x4::QuaternionToEuler(q.x, q.y, q.z, q.w);
	float ax = RAD_TO_DEG(z.x);
	float ay = RAD_TO_DEG(z.y);
	float az = RAD_TO_DEG(z.z);
	ax = ClampAngle(ax);
	ay = ClampAngle(ay);
	az = ClampAngle(az);
	return ZobVector3(ax, ay, az);
}

float ZobPhysicComponent::ClampAngle(float a) const
{
	int i = abs((int)a / 180);
	a -= 180.0f * (float)i;
	return a;
}

void ZobPhysicComponent::Update()
{
	UpdateShapeType();
	UpdateColliderSize();
	bool bPhysicRunning = DirectZob::GetInstance()->IsPhysicPlaying();
	if (m_rigidBody)
	{
		if(bPhysicRunning && m_type == ePhysicComponentType_dynamic)
		{
			m_worldTransform = m_rigidBody->getTransform();
		}
		else
		{
			m_rigidBody->setTransform(m_worldTransform);
		}
	}
	m_totalScale = Vector3(1, 1, 1);
}

ZobMatrix4x4 ZobPhysicComponent::GetRotationMatrix() const
{
	reactphysics3d::decimal* mat = (reactphysics3d::decimal*)malloc(sizeof(decimal) * 16);
	m_worldTransform.getOpenGLMatrix(mat);
	ZobMatrix4x4 m;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for(int j=0; j<4; j++)
		{
			m.SetData(i, j, mat[k]);
			k++;
		}
	}
	delete mat;
	m.SetPosition(0, 0, 0);
	return m;
}

void ZobPhysicComponent::AddBoxCollider()
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	Vector3 h = Vector3(m_halfExtends.x, m_halfExtends.y, m_halfExtends.z);
	BoxShape* s = pc->createBoxShape(h);
	m_shapeType = eShapeType::eShapeType_box;
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddSphereCollider()
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	SphereShape* s= pc->createSphereShape(m_radius);
	m_shapeType = eShapeType::eShapeType_sphere;
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddCapsuleCollider()
{
	PhysicsCommon* pc = DirectZob::GetInstance()->GetPhysicsEngine()->GetPhysicsCommon();
	CapsuleShape* s = pc->createCapsuleShape(m_radius, m_height);
	AddColliderInternal(s);
}

void ZobPhysicComponent::AddColliderInternal(CollisionShape* c)
{
	if (m_collider)
	{
		m_rigidBody->removeCollider(m_collider);
		m_collider = NULL;
	}
	Transform t = Transform::identity();
	//t = m_rigidBody->getTransform();
	m_collider = m_rigidBody->addCollider(c, t);

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

void ZobPhysicComponent::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation)
{
	ZobMatrix4x4 mat;
	mat.SetPosition(position);
	mat.SetRotation(rotation);
	uint c = 0x00FF00;
	bool bold = false;
	Engine* e = DirectZob::GetInstance()->GetEngine();
	switch (m_shapeType)
	{
	case eShapeType::eShapeType_sphere:
		e->QueueSphere(camera, &mat, m_radius, c, bold, false);
		break;
	case eShapeType::eShapeType_box:
		//TODO ERROR : ajouter le pivot !
		e->QueueBox(camera, &mat, &m_halfExtends, position, c, bold, false);
		break;
	case eShapeType::eShapeType_capsule:
	case eShapeType::eShapeType_convexMesh:
	default:
		break;
	}
}

Quaternion ZobPhysicComponent::QuaternionFromAxisAngle(Vector3* axis, float angle)
{
	float s = sinf(angle / 2.0f);
	Quaternion q;
	q.x = axis->x * s;
	q.y = axis->y * s;
	q.z = axis->z * s;
	q.w = cosf(angle / 2.0f);
	return q;
}

bool ZobPhysicComponent::SetRadius(float f)
{
	m_radius = f;
	m_bUpdateSize = true;
	return true;
}

bool ZobPhysicComponent::SetHalfextends(float x, float y, float z)
{
	m_halfExtends.x = x;
	m_halfExtends.y = y;
	m_halfExtends.z = z;
	m_bUpdateSize = true;
	return true;
}

bool ZobPhysicComponent::SetHeight(float h)
{
	m_height = h;
	m_bUpdateSize = true;
	return true;
}

bool ZobPhysicComponent::UpdateColliderSize()
{
	if (m_bUpdateSize && m_collider)
	{
		m_bUpdateSize = false;
		CapsuleShape* c = dynamic_cast<CapsuleShape*>(m_collider->getCollisionShape());
		if (c)
		{
			c->setHeight((decimal)m_height);
			c->setRadius(m_radius);
			return true;
		}
		BoxShape* b = dynamic_cast<BoxShape*>(m_collider->getCollisionShape());
		if (b)
		{
			Vector3 he = Vector3(m_halfExtends.x, m_halfExtends.y, m_halfExtends.z);
			b->setHalfExtents(he);
			return true;
		}
		SphereShape* s = dynamic_cast<SphereShape*>(m_collider->getCollisionShape());
		if (s)
		{
			s->setRadius(m_radius);
			return true;
		}
	}
	return false;
}
