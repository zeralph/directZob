#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"
#include "Rendering/ZobMatrix4x4.h"


ZobPhysicComponent::ZobPhysicComponent(ZobObject* z)
{
	m_zobObject = z;
	BodyType rigidBodyType = rp3d::BodyType::STATIC;
	bool rigidBodyActive = false;
	m_scaleWithObject = true;
	m_collider = NULL;
	m_bUpdateSize = true;
	m_collisionBody = DirectZob::GetInstance()->GetPhysicsEngine()->CreateCollisionBody(&ZobVector3::Vector3Zero, &ZobVector3::Vector3Zero);;
	m_collisionBody->setIsActive(true);
//	m_rigidBody->setIsAllowedToSleep(false);
	m_collider = NULL;
	m_lastCollision.Reset();
	DirectZob::GetInstance()->GetPhysicsEngine()->AddBody(this);
	m_editorLocalPosition = ZobVector3(m_localTransform.getPosition().x, m_localTransform.getPosition().y, m_localTransform.getPosition().z);
	m_editorLocalRotation = GetLocalOrientation();
}

ZobPhysicComponent::~ZobPhysicComponent()
{
	DirectZob::GetInstance()->GetPhysicsEngine()->RemoveBody(this);
	DirectZob::GetInstance()->GetPhysicsEngine()->DestroyCollisionBody(m_collisionBody);
	m_collisionBody = NULL;}

void ZobPhysicComponent::Init(const ZobVector3* position, const ZobVector3* rotation)
{
	m_localTransform = Transform::identity();
	m_localTransform.setPosition(Vector3(position->x, position->y, position->z));
	Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(rotation->x), DEG_TO_RAD(rotation->y), DEG_TO_RAD(rotation->z));
	m_localTransform.setOrientation(q);
	m_scale = Vector3(1, 1, 1);
	m_totalScale = Vector3(1, 1, 1);
	m_editorLocalScale.x = m_totalScale.x;
	m_editorLocalScale.y = m_totalScale.y;
	m_editorLocalScale.z = m_totalScale.z;
	SetWorldTransform( GetParentWorldTransform() * m_localTransform);
	Update();
}

Transform ZobPhysicComponent::GetParentWorldTransform() const
{
	const ZobObject* zp = m_zobObject->GetParent();
	if (zp)
	{
		return zp->GetPhysicComponent()->GetWorldTransform();
	}
	else
	{
		return m_localTransform;
	}
}

void ZobPhysicComponent::SetLocalTransform(Transform t) 
{
	m_localTransform = t; 
	m_editorLocalPosition = ZobVector3(m_localTransform.getPosition().x, m_localTransform.getPosition().y, m_localTransform.getPosition().z);
	m_editorLocalRotation = GetLocalOrientation();
};

void ZobPhysicComponent::SetWorldTransform(Transform t)
{ 
	const Transform t2 = GetParentWorldTransform();
	m_localTransform = t2.getInverse() * t;
	m_editorLocalPosition = ZobVector3(m_localTransform.getPosition().x, m_localTransform.getPosition().y, m_localTransform.getPosition().z);
	m_editorLocalRotation = GetLocalOrientation();
}

Transform ZobPhysicComponent::GetWorldTransform() const 
{ 
	Transform parentTransform = GetParentWorldTransform();
	parentTransform = parentTransform * m_localTransform;
	return parentTransform;
};

ZobVector3 ZobPhysicComponent::GetPosition() const
{
	const Vector3 v = GetWorldTransform().getPosition();
	return ZobVector3(v.x, v.y, v.z);
}

void ZobPhysicComponent::SetPosition(float x, float y, float z)
{
	Vector3 v = Vector3(x, y, z);
	Transform t = GetWorldTransform();
	t.setPosition(v);
	SetWorldTransform(t);
}

void ZobPhysicComponent::SetQuaternion(const ZobVector3* left, const ZobVector3* up, const ZobVector3* fw)
{
	reactphysics3d::Matrix3x3 m;
	m.setAllValues(left->x, up->x, fw->x,
		left->y, up->y, fw->y,
		left->z, up->z, fw->z);
	Quaternion q = Quaternion(m);
	Transform w = GetWorldTransform();
	Transform t = Transform(w.getPosition(), q);
	SetWorldTransform(t);
}

void ZobPhysicComponent::SetQuaternion(float x, float y, float z, float w)
{
	Quaternion q = Quaternion(x, y, z, w);
	Transform wt = GetWorldTransform();
	Transform t = Transform(wt.getPosition(), q);
	SetWorldTransform(t);
}
void ZobPhysicComponent::LookAt(const ZobVector3* target)
{
	Transform t = GetWorldTransform();
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
	Transform t = GetWorldTransform();
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
		SetWorldTransform(t);
	}
}
void ZobPhysicComponent::SetTotalScale(float x, float y, float z)
{ 
	if (m_totalScale.x != x || m_totalScale.y != y || m_totalScale.z != z)
	{
		m_totalScale.x = x;
		m_totalScale.y = y;
		m_totalScale.z = z;
		m_editorLocalScale.x = m_totalScale.x;
		m_editorLocalScale.y = m_totalScale.y;
		m_editorLocalScale.z = m_totalScale.z;
		m_bUpdateSize = true;
	}
}

void ZobPhysicComponent::SetLocalOrientation(float x, float y, float z)
{
	float rx = DEG_TO_RAD(x);
	float ry = DEG_TO_RAD(y);
	float rz = DEG_TO_RAD(z);
	Transform t = GetWorldTransform();
	Quaternion q;
	//q = Quaternion::fromEulerAngles((dx), (dy), (dz));
	ZobVector3 v = ZobMatrix4x4::EulerToQuaternion(rx, ry, rz);
	q.x = v.x;
	q.y = v.y;
	q.z = v.z;
	q.w = v.w;
	Quaternion q2 = t.getOrientation();
	q = q2 * q;
	m_localTransform.setOrientation(q);
	m_editorLocalRotation = GetLocalOrientation();
}

void ZobPhysicComponent::SetWorldOrientation(float x, float y, float z)
{
	float rx = DEG_TO_RAD(x);
	float ry = DEG_TO_RAD(y);
	float rz = DEG_TO_RAD(z);
	Transform t = GetWorldTransform();
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
	SetWorldTransform(t);
	m_editorLocalRotation = GetLocalOrientation();
}

ZobVector3 ZobPhysicComponent::GetLocalOrientation() const
{
	Quaternion q = m_localTransform.getOrientation();
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

ZobVector3 ZobPhysicComponent::GetWorldOrientation() const
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

void ZobPhysicComponent::EditorUpdate()
{
	Vector3 p = m_localTransform.getPosition();
	Vector3 v;
	v.x = m_editorLocalPosition.x;
	v.y = m_editorLocalPosition.y;
	v.z = m_editorLocalPosition.z;
	if (p.x != v.x || p.y != v.y || p.z != v.z)
	{
		m_localTransform.setPosition(v);
	}
	ZobVector3 z = GetLocalOrientation();
	v.x = m_editorLocalRotation.x;
	v.y = m_editorLocalRotation.y;
	v.z = m_editorLocalRotation.z;
	if (z.x != v.x || z.y != v.y || z.z != v.z)
	{
		Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(v.x), DEG_TO_RAD(v.y), DEG_TO_RAD(v.z));
		m_localTransform.setOrientation(q);
	}
}

void ZobPhysicComponent::Update()
{
	assert(m_collisionBody);
	bool bPhysicRunning = DirectZob::GetInstance()->IsPhysicPlaying();
	if (m_collisionBody)
	{
		m_collisionBody->setIsActive(true);
		if(false)//m_bodyType == ZobObject::eBodyType_dynamic)
		{
			SetWorldTransform (m_collisionBody->getTransform());
		}
		else
		{
			Transform t = GetWorldTransform();
			m_collisionBody->setTransform(t);
			m_editorLocalRotation = GetLocalOrientation();
			m_editorLocalPosition.x = m_localTransform.getPosition().x;
			m_editorLocalPosition.y = m_localTransform.getPosition().y;
			m_editorLocalPosition.z = m_localTransform.getPosition().z;
		}
	}
	m_totalScale = Vector3(1, 1, 1);
}

ZobMatrix4x4 ZobPhysicComponent::GetRotationMatrix() const
{
	reactphysics3d::decimal* mat = (reactphysics3d::decimal*)malloc(sizeof(decimal) * 16);
	GetWorldTransform().getOpenGLMatrix(mat);
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

void ZobPhysicComponent::SaveTransform()
{

	m_savedTransform = m_collisionBody->getTransform();

}

void ZobPhysicComponent::RestoreTransform()
{
	m_collisionBody->setTransform(m_savedTransform);
}

void ZobPhysicComponent::ResetPhysic()
{
	if(m_collisionBody->isActive())
	{
		m_collisionBody->setIsActive(false);
		m_collisionBody->setIsActive(true);
	}
}

void ZobPhysicComponent::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation)
{
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

void ZobPhysicComponent::OnCollide(collision coll)
{
	if (m_lastCollision.handled)
	{
		m_lastCollision = coll;
		m_lastCollision.handled = false;
	}
}
