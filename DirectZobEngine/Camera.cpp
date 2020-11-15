#include "Camera.h"
#include "DirectZob.h"
#include <math.h>
#include <mutex>
#include "DirectZob.h"
#include "ZobPhysicComponent.h"

static std::mutex g_update_camera_mutex;
static float ee = 0.0f;

static ZobVector3 sRayDbg;
static ZobVector3 sRayDbg2;

Camera::Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, name, parent)
{
	//RecomputeVectors();
	m_fov = fov;
	m_tagetMode = eTarget_none;
	sRayDbg = ZobVector3(1000, 1000, 1000);
	sRayDbg2 = ZobVector3(1000, 1000, 1000);
	//m_nextTranslation = m_translation;
}

Camera::Camera(TiXmlElement* node, ZobObject* parent)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, node, parent)
{
	TiXmlElement * f = node->FirstChildElement("Fov");
	float fov = f ? atof(f->GetText()) : 45.0f; 
	m_fov = fov;
	//m_nextTranslation = m_translation;
	m_tagetMode = eTarget_none;
}

Camera::~Camera()
{
	DirectZob::LogInfo("Delete Camera %s", m_name.c_str());
	DirectZob::AddIndent();
	DirectZob::GetInstance()->GetCameraManager()->RemoveCamera(this);
	DirectZob::RemoveIndent();
}

void Camera::DrawGizmos(const Camera* camera, Core::Engine* engine)
{
	if (GetType() == ZOBGUID::type_editor)
	{
		//		return;
	}
	ZobObject::DrawGizmos(camera, engine);
	ZobVector3 v0 = ZobVector3(-2, 1, 1);
	ZobVector3 v1 = ZobVector3(2, 1, 1);
	ZobVector3 v2 = ZobVector3(-2, -1, 1);
	ZobVector3 v3 = ZobVector3(2, -1, 1);
	m_rotationScaleMatrix.Mul(&v0);
	m_rotationScaleMatrix.Mul(&v1);
	m_rotationScaleMatrix.Mul(&v2);
	m_rotationScaleMatrix.Mul(&v3);
	const ZobVector3* p = GetPosition();
	v0 = v0 + p;
	v1 = v1 + p;
	v2 = v2 + p;
	v3 = v3 + p;
	uint c = 0x000000FF;
	uint c2 = 0x0000FFFF;
	if (this->GetName() != "EditorCamera")
	{
		engine->QueueLine(camera, &v0, &v1, c, true);
		engine->QueueLine(camera, &v1, &v3, c, true);
		engine->QueueLine(camera, &v2, &v3, c2, true);
		engine->QueueLine(camera, &v2, &v0, c, true);
		engine->QueueLine(camera, p, &v1, c, true);
		engine->QueueLine(camera, p, &v2, c, true);
		engine->QueueLine(camera, p, &v3, c, true);
		engine->QueueLine(camera, p, &v0, c, true);
	}

	ZobMatrix4x4 z;
	z.Identity();
	z.Identity();
	z.AddTranslation(sRayDbg);
	//engine->QueueSphere(camera, &z, 0.1f, c2, true);
	ZobVector3 q = sRayDbg - sRayDbg2;
	q = sRayDbg2 + q * 500.0f;
	engine->QueueLine(camera, &sRayDbg, &q, c2, true);
}

void Camera::Zoom(float z)
{
	//z = z / abs(z);
	ZobVector3 v = m_forward;
	if (m_tagetMode != eTarget_none)
	{
		if (m_tagetMode == eTarget_Vector)
		{
			v = GetPosition();
			v = v - m_targetVector;
			
		}
		else if (m_tagetMode == eTarget_Object && m_targetObject)
		{
			v = GetPosition();
			v = v - m_targetObject->GetPosition();
		}
	}
	if (v.sqrtLength() > abs(z) || z < 0.0f)
	{
		v.Normalize();
		v = v * (-z);
		v = v + GetPosition();
		SetPosition(v.x, v.y, v.z);
	}
}

bool Camera::GetTargetVector(ZobVector3* t)
{
	if (m_tagetMode != eTarget_none)
	{
		if (m_tagetMode == eTarget_Vector)
		{
			t->Copy(&m_targetVector);
			return true;
		}
		else if (m_tagetMode == eTarget_Object && m_targetObject)
		{
			const ZobVector3* merde = m_targetObject->GetPosition();
			ZobVector3 v = ZobVector3(merde);
			t->Copy(&v);
			return true;
		}
	}
	return false;
}

void Camera::RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle, bool recomputeVectors)
{
	angle = angle* M_PI / 180.0;
	Transform t, t2;
	Vector3 paxis = Vector3(axis->x, axis->y, axis->z);
	t = Transform::identity();
	Vector3 p = Vector3(point->x, point->y, point->z);
	Quaternion q = m_physicComponent->QuaternionFromAxisAngle(&paxis, angle);
	q.normalize();
	t = Transform(Vector3::zero(), q);
	t2 = Transform::identity();
	Vector3 localPos = m_physicComponent->GetLocalTransform().getPosition();
	t2.setPosition(localPos - p);
	t2 = t * t2;
	localPos = t2.getPosition();
	t2.setPosition(p + localPos);
	m_physicComponent->SetLocalTransform(t2);
	LookAt(point);
}

void Camera::RecomputeFLUVectors(const ZobVector3* forwardV, const ZobVector3* upV)
{
	ZobVector3 fw = forwardV;
	fw.Normalize();
	ZobVector3 up = upV;
	ZobVector3 left = ZobVector3::Cross(&up, forwardV);
	left.Normalize();
	up = ZobVector3::Cross(&fw, &left);
	up.Normalize();
	m_forward = fw;
	m_up = up;
	m_left = left;
}

void Camera::Move(float dx, float dy, bool moveTargetVector)
{
	ZobVector3 vl = ZobVector3(m_left);
	vl = vl * ((float)-dx / 20.0f);
	vl.y = 0;

	ZobVector3 vf = ZobVector3(m_forward);
	
	vf = vf * ((float)dy / 20.0f);
	vf.y = 0;
	//m_nextTranslation = m_nextTranslation - (vl + vf);
	ZobVector3 v = GetPosition();
	v = v - (vl + vf);
	SetPosition(v.x, v.y, v.z);
	if (moveTargetVector)
	{
		m_targetVector = m_targetVector - (vl + vf);
		//m_targetVector.Normalize();
	}
}

//void Camera::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
void Camera::Update()
{
	ZobObject::Update();
	ZobVector3 v = GetPosition();
	if (m_tagetMode != eTarget_none)
	{
		if (m_tagetMode == eTarget_Vector &&  m_targetVector != v)
		{
//			RecomputeFLUVectors(&v, &ZobVector3::Vector3Y);
//			LookAt(&m_forward, &m_left, &m_up);
			UpdateViewProjectionMatrix(GetPosition(), &m_targetVector, &ZobVector3::Vector3Y);
		}
		else if (m_tagetMode == eTarget_Object && m_targetObject)
		{
			//todo : cette section est a revoir
			const ZobVector3* p = GetPosition();
			const ZobVector3* tp = m_targetObject->GetPosition();
			ZobVector3 v = ZobVector3(tp);
			if( v != GetPosition())
			{
				v = ZobVector3(tp);
				v = v - p;
				v.Normalize();
				//m_forward = v;
				//v = ZobVector3::GetAnglesFromVector(v);
				v = ZobMatrix4x4::QuaternionToEuler(v.x, v.z, v.z, 45.0f);
				SetRotation(v.x, v.y, v.z);
				RecomputeFLUVectors(&v, &ZobVector3::Vector3Y);
			}
			UpdateViewProjectionMatrix(GetPosition(), &m_targetVector, &ZobVector3::Vector3Y);
		}
		else if ((m_tagetMode == eTarget_FPS))
		{
			UpdateViewProjectionMatrix(&v, 0.0f, 0.0f);
		}
		//this will update the pysicnode orientation according to the lookAt
		LookAt(&m_forward, &m_left, &m_up);
	}
	else 
	{
		UpdateViewProjectionMatrix(&v);
	}
}
void Camera::UpdateViewProjectionMatrix(const ZobVector3* eyeV)
{
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);
	SetViewMatrix(&m_left, &m_up, &m_forward, eyeV);
}
void Camera::UpdateViewProjectionMatrix(const ZobVector3* eyeV, const ZobVector3* targetV, const ZobVector3* upV)
{
	//g_update_camera_mutex.lock();
	m_viewTransaltion = GetPosition();
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);	
	ZobVector3 up = upV;
	ZobVector3 forward = targetV;
	forward.x -= eyeV->x;
	forward.y -= eyeV->y;
	forward.z -= eyeV->z;
	forward.Normalize();
	ZobVector3 left = ZobVector3::Cross(&up, &forward);
	left.Normalize();
	up = ZobVector3::Cross(&forward, &left);
	up.Normalize();
	SetViewMatrix(&left, &up, &forward, eyeV);
	m_left = left;
	m_forward = forward;
	m_up = up;
	//todo : clean that
	ZobMatrix4x4::InvertMatrix4(m_rotationScaleMatrix, m_invModelMatrix);
	ZobMatrix4x4::InvertMatrix4(m_modelMatrix, m_invModelMatrix);
	ZobMatrix4x4::InvertMatrix4(m_projMatrix, m_invProjectionMatrix);
	ZobMatrix4x4::InvertMatrix4(m_viewRotMatrix, m_invViewMatrix);
	
	//g_update_camera_mutex.unlock();
}

void Camera::UpdateViewProjectionMatrix(const ZobVector3* eye, const float pitch, const float yaw)
{
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);

	float cosPitch = cos(pitch);
	float sinPitch = sin(pitch);
	float cosYaw = cos(yaw);
	float sinYaw = sin(yaw);

	ZobVector3 left = ZobVector3( cosYaw, 0, -sinYaw );
	ZobVector3 up = ZobVector3( sinYaw * sinPitch, cosPitch, cosYaw * sinPitch );
	ZobVector3 forward = ZobVector3( sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw );

	SetViewMatrix(&left, &up, &forward, eye);
	m_left = left;
	m_forward = forward;
	m_up = up;
}

void Camera::SetViewMatrix(const ZobVector3& left, const ZobVector3& up, const ZobVector3& fw, const ZobVector3& p)
{
	float x = -ZobVector3::Dot(&left, &p);
	float y = -ZobVector3::Dot(&up, &p);
	float z = -ZobVector3::Dot(&fw, &p);
	m_viewRotMatrix.SetData(0, 0, left.x);
	m_viewRotMatrix.SetData(0, 1, left.y);
	m_viewRotMatrix.SetData(0, 2, left.z);
	m_viewRotMatrix.SetData(0, 3, x);//-p.x);

	m_viewRotMatrix.SetData(1, 0, up.x);
	m_viewRotMatrix.SetData(1, 1, up.y);
	m_viewRotMatrix.SetData(1, 2, up.z);
	m_viewRotMatrix.SetData(1, 3, y);//-p.y);

	m_viewRotMatrix.SetData(2, 0, fw.x);
	m_viewRotMatrix.SetData(2, 1, fw.y);
	m_viewRotMatrix.SetData(2, 2, fw.z);
	m_viewRotMatrix.SetData(2, 3, z);//-p.z);

	m_viewRotMatrix.SetData(3, 0, 0);
	m_viewRotMatrix.SetData(3, 1, 0);
	m_viewRotMatrix.SetData(3, 2, 0);
	m_viewRotMatrix.SetData(3, 3, 1);
	//-------------------------------------------------------

}

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float zNear, const float zFar)
{
	const float ar = width / height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = -tanf(angleOfView / 2.0 * M_PI / 180.0);

	float a = 1.0f / (tanHalfFOV * ar);
	float b = 1.0f / tanHalfFOV;
	float c = (-zNear - zFar) / zRange;
	float d = 2.0f * zFar * zNear / zRange;
	float e = 1.0f;

	m_projMatrix.SetData(0, 0, a);
	m_projMatrix.SetData(0, 1, 0.0f);
	m_projMatrix.SetData(0, 2, 0.0f);
	m_projMatrix.SetData(0, 3, 0.0f);

	m_projMatrix.SetData(1, 0, 0.0f);
	m_projMatrix.SetData(1, 1, b);
	m_projMatrix.SetData(1, 2, 0.0f);
	m_projMatrix.SetData(1, 3, 0.0f);

	m_projMatrix.SetData(2, 0, 0.0f);
	m_projMatrix.SetData(2, 1, 0.0f);
	m_projMatrix.SetData(2, 2, c);
	m_projMatrix.SetData(2, 3, d);

	m_projMatrix.SetData(3, 0, 0.0f);
	m_projMatrix.SetData(3, 1, 0.0f);
	m_projMatrix.SetData(3, 2, e);
	m_projMatrix.SetData(3, 3, 0.0f);

	//ZobMatrix4x4::InvertMatrix4(m_projMatrix, m_invProjectionMatrix);

	m_invProjectionMatrix.SetData(0, 0, 1.0f / a);
	m_invProjectionMatrix.SetData(0, 1, 0.0f);
	m_invProjectionMatrix.SetData(0, 2, 0.0f);
	m_invProjectionMatrix.SetData(0, 3, 0.0f);
	
	m_invProjectionMatrix.SetData(1, 0, 0.0f);
	m_invProjectionMatrix.SetData(1, 1, 1.0f / b);
	m_invProjectionMatrix.SetData(1, 2, 0.0f);
	m_invProjectionMatrix.SetData(1, 3, 0.0f);
	
	m_invProjectionMatrix.SetData(2, 0, 0.0f);
	m_invProjectionMatrix.SetData(2, 1, 0.0f);
	m_invProjectionMatrix.SetData(2, 2, 0.0f);
	m_invProjectionMatrix.SetData(2, 3, 1.0f / e);
	
	m_invProjectionMatrix.SetData(3, 0, 0.0f);
	m_invProjectionMatrix.SetData(3, 1, 0.0f);
	m_invProjectionMatrix.SetData(3, 2, 1.0f / d);
	m_invProjectionMatrix.SetData(3, 3, -c / (d*e) );

}

//v is homogenous
Camera::Ray Camera::From2DToWorld(const ZobVector3* v2d)
{
	ZobVector3 v = v2d;
	m_invProjectionMatrix.Mul(&v);
	m_invViewMatrix.Mul(&v);
	v.Normalize();	
	v.x += this->GetWorldPosition().x;
	v.y += this->GetWorldPosition().y;
	v.z += this->GetWorldPosition().z;
	Ray r;
	r.p = this->GetWorldPosition();
	r.n = v - r.p;
	return r;
}

void Camera::From2DToWorldOnPlane(ZobVector3* v2d, ZobVector3* p0, ZobVector3* pn)
{
	Camera::Ray r = From2DToWorld(v2d);
	ZobVector3 l0 = this->GetWorldPosition();
	ZobVector3 lv = v2d - l0;
	ZobVector3 i = DirectZob::GetInstance()->GetEngine()->LinePlaneIntersection(p0, pn, &r.p, &r.n);
	v2d->x = i.x;
	v2d->y = i.y;
	v2d->z = i.z;
	DirectZob::GetInstance()->GetEngine()->QueueLine(this, this->GetPosition(), v2d, 0xFF00FF, true);
}

TiXmlNode* Camera::SaveUnderNode(TiXmlNode* node)
{
	char tmpBuffer[256];
	TiXmlNode* n = ZobObject::SaveUnderNode(node);
	TiXmlElement* ne = (TiXmlElement*)n;
	ne->SetAttribute("type", "camera");
	TiXmlText t("");
	TiXmlElement fov = TiXmlElement("Fov");
	_snprintf_s(tmpBuffer, 256, "%.2f", GetFov());
	t.SetValue(tmpBuffer);
	fov.InsertEndChild(t);
	ne->InsertEndChild(fov);
	return n;
}
