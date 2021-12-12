#include "ZobPhysicComponent.h"
#include "ZobPhysicsEngine.h"
#include "DirectZob.h"
#include "Rendering/ZobMatrix4x4.h"
#include <cfloat>

ZobPhysicComponent::ZobPhysicComponent(ZobObject* z)
{
	m_zobObject = z;
	BodyType rigidBodyType = rp3d::BodyType::STATIC;
	bool rigidBodyActive = false;
	m_collider = NULL;
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

void ZobPhysicComponent::Init()
{
	m_localTransform = Transform::identity();
	m_localTransform.setPosition(Vector3(m_editorLocalPosition.x, m_editorLocalPosition.y, m_editorLocalPosition.z));
	Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(m_editorLocalRotation.x), DEG_TO_RAD(m_editorLocalRotation.y), DEG_TO_RAD(m_editorLocalRotation.z));
	m_localTransform.setOrientation(q);
	m_localScale = Vector3(1, 1, 1);
	m_localScale.x = m_editorLocalScale.x;
	m_localScale.y = m_editorLocalScale.y;
	m_localScale.z = m_editorLocalScale.z;
	SetWorldTransform( GetParentWorldTransform() * m_localTransform);
	Update();
}

Vector3 ZobPhysicComponent::GetParentWorldScale() const
{
	const ZobObject* zp = m_zobObject->GetParent();
	if (zp)
	{
		return zp->GetPhysicComponent()->GetWorldScale();
	}
	else
	{
		return m_localScale;
	}
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
	assert(t.getPosition().isFinite());
	m_localTransform = t; 
	m_editorLocalPosition = ZobVector3(m_localTransform.getPosition().x, m_localTransform.getPosition().y, m_localTransform.getPosition().z);
	m_editorLocalRotation = GetLocalOrientation();
	m_editorWorldPosition.x = t.getPosition().x;
	m_editorWorldPosition.y = t.getPosition().y;
	m_editorWorldPosition.z = t.getPosition().z;
	m_editorWorldRotation = GetWorldOrientation();
};

void ZobPhysicComponent::SetWorldTransform(Transform t)
{ 
	const Transform t2 = GetParentWorldTransform();
	m_localTransform = t2.getInverse() * t;
	Vector3 p = m_localTransform.getPosition();
	Vector3 ps = GetParentWorldScale();
	p.x /= ps.x;
	p.y /= ps.y;
	p.z /= ps.z;
	m_localTransform.setPosition(p);
	assert(m_localTransform.getPosition().isFinite());
	m_editorLocalPosition = ZobVector3(m_localTransform.getPosition().x, m_localTransform.getPosition().y, m_localTransform.getPosition().z);
	m_editorLocalRotation = GetLocalOrientation();
}

void ZobPhysicComponent::SetWorldScale(float x, float y, float z)
{
	Vector3 ps = GetParentWorldScale();
	m_localScale.x = x / ps.x;
	m_localScale.y = y / ps.y;
	m_localScale.z = z / ps.z;
}

Vector3 ZobPhysicComponent::GetWorldScale() const
{
	Vector3 ps = GetParentWorldScale();
	ps.x *= m_localScale.x;
	ps.y *= m_localScale.y;
	ps.z *= m_localScale.z;
	return ps;
}

Transform ZobPhysicComponent::GetWorldTransform() const 
{ 
	Transform parentTransform = GetParentWorldTransform();
	Transform t = m_localTransform;
	Vector3 v = t.getPosition();
	Vector3 s = GetParentWorldScale();
	assert(v.isFinite());
	v.x *= s.x;
	v.y *= s.y;
	v.z *= s.z;
	t.setPosition(v);
	parentTransform = parentTransform * t;
	return parentTransform;
};

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
	//Quaternion q2 = t.getOrientation();
	//q = q2 * q;
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
	/*
	Vector3 p = m_localTransform.getPosition();
	Vector3 v;
	v.x = m_editorLocalPosition.x;
	v.y = m_editorLocalPosition.y;
	v.z = m_editorLocalPosition.z;
	if (p.x != v.x || p.y != v.y || p.z != v.z)
	{
		m_localTransform.setPosition(v);
		v.x = m_editorLocalRotation.x;
		v.y = m_editorLocalRotation.y;
		v.z = m_editorLocalRotation.z;
	}
	ZobVector3 z = GetLocalOrientation();
	if (z.x != v.x || z.y != v.y || z.z != v.z)
	{
		Quaternion q = Quaternion::fromEulerAngles(DEG_TO_RAD(v.x), DEG_TO_RAD(v.y), DEG_TO_RAD(v.z));
		m_localTransform.setOrientation(q);
	}
	*/
}

void ZobPhysicComponent::WorldOrientationToAxis(ZobVector3& left, ZobVector3& up, ZobVector3& forward) const
{
	Quaternion q = GetWorldTransform().getOrientation();
	Vector3 l = q * Vector3(1, 0, 0);
	Vector3 u = q * Vector3(0, 1, 0);
	Vector3 f = q * Vector3(0, 0, 1);
	l.normalize();
	u.normalize();
	f.normalize();
	left = ZobVector3(l.x, l.y, l.z);
	forward = ZobVector3(f.x, f.y, f.z);
	up = ZobVector3(u.x, u.y, u.z);
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
			//editor world variables update
			m_editorWorldPosition.x = t.getPosition().x;
			m_editorWorldPosition.y = t.getPosition().y;
			m_editorWorldPosition.z = t.getPosition().z;
			ZobVector3 a = GetWorldOrientation();
			m_editorWorldRotation.x = a.x;
			m_editorWorldRotation.y = a.y;
			m_editorWorldRotation.z = a.z;
			m_editorWorldScale.x = GetWorldScale().x;
			m_editorWorldScale.y = GetWorldScale().y;
			m_editorWorldScale.z = GetWorldScale().z;
		}
	}
	//m_totalScale = Vector3(1, 1, 1);
}

ZobMatrix4x4 ZobPhysicComponent::GetModelMatrix() const
{
	reactphysics3d::decimal mat[16];// = (reactphysics3d::decimal*)malloc(sizeof(decimal) * 16);
	GetWorldTransform().getOpenGLMatrix(mat);
	ZobMatrix4x4 m;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for(int j=0; j<4; j++)
		{
			m.SetData(j, i, mat[k]);
			k++;
		}
	}
	//delete mat;
	//m.SetPosition(0, 0, 0);
	Vector3 s = GetWorldScale();
	m.SetScale(s.x, s.y, s.z);
	return m;
}

ZobMatrix4x4 ZobPhysicComponent::GetRotationMatrix() const
{
	Transform t = GetLocalTransform();
	t.setPosition(Vector3(0, 0, 0));
	reactphysics3d::decimal mat[16];// = (reactphysics3d::decimal*)malloc(sizeof(decimal) * 16);
	t.getOpenGLMatrix(mat);
	ZobMatrix4x4 m;
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.SetData(j, i, mat[k]);
			k++;
		}
	}
	//delete mat;
	//m.SetPosition(0, 0, 0);
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

void ZobPhysicComponent::GetLocalAxisAngleRotation(ZobVector3& axis, float& angle)
{
	Quaternion q = m_localTransform.getOrientation();
	Vector3 a;
	float f;
	q.getRotationAngleAxis(f, a);
	axis.x = a.x;
	axis.y = a.y;
	axis.z = a.z;
	angle = f;
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
