#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"
#include "Rendering/ZobMatrix4x4.h"


ZobPhysicComponent::ZobPhysicComponent(TiXmlNode* t)
{
	BodyType rigidBodyType = rp3d::BodyType::STATIC;
	bool rigidBodyActive = false;
	m_type = ePhysicComponentType_none;
	if (t)
	{
		TiXmlElement* p = (TiXmlElement*)t;
		m_type = (ePhysicComponentType)atoi(p->Attribute("type"));
		TiXmlElement* b = p->FirstChildElement("RigidBody");
		rigidBodyType = (BodyType)atoi(b->Attribute("type"));
		rigidBodyActive = atoi(b->Attribute("active")) == 1 ? true : false;
		TiXmlElement* c = p->FirstChildElement("Collider");
		m_shapeDraw._type = (sShapeDraw::eShapeType)atoi(c->Attribute("type"));
		m_shapeDraw._radius = (float)atof(c->Attribute("radius"));
		m_shapeDraw._height = (float)atof(c->Attribute("height"));
		m_shapeDraw._halfExtends.x = (float)atof(c->Attribute("halfExtends_x"));
		m_shapeDraw._halfExtends.y = (float)atof(c->Attribute("halfExtends_y"));
		m_shapeDraw._halfExtends.z = (float)atof(c->Attribute("halfExtends_z"));
	}
	m_rigidBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateRigidBody(&ZobVector3::Vector3Zero, &ZobVector3::Vector3Zero);
	m_rigidBody->setType(rigidBodyType);
	m_rigidBody->setIsActive(rigidBodyActive);
	CreateCollider();
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

void ZobPhysicComponent::CreateCollider()
{
	m_collider = NULL;
	switch (m_shapeDraw._type)
	{
	case sShapeDraw::eShapeType::eShapeType_box:
		AddBoxCollider(&m_shapeDraw._halfExtends);
		break;
	case sShapeDraw::eShapeType::eShapeType_sphere:
		AddSphereCollider(m_shapeDraw._radius);
		break;
	default:
		break;
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

void ZobPhysicComponent::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation)
{
	ZobMatrix4x4 mat;
	mat.SetPosition(position);
	mat.SetRotation(rotation);
	uint c = 0x00FF00;
	bool bold = false;
	Engine* e = DirectZob::GetInstance()->GetEngine();
	switch (m_shapeDraw._type)
	{
	case sShapeDraw::eShapeType::eShapeType_sphere:
		e->QueueSphere(camera, &mat, m_shapeDraw._radius, c, bold, false);
		break;
	case sShapeDraw::eShapeType::eShapeType_box:
		//TODO ERROR : ajouter le pivot !
		e->QueueBox(camera, &mat, &m_shapeDraw._halfExtends, position, c, bold, false);
		break;
	case sShapeDraw::eShapeType::eShapeType_capsule:
	case sShapeDraw::eShapeType::eShapeType_convexMesh:
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