#include "Camera.h"
#include "DirectZob.h"
#include <math.h>
#include <mutex>
#include "DirectZob.h"

static std::mutex g_update_camera_mutex;
static float ee = 0.0f;

Camera::Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, name, NULL, parent)
{
	//RecomputeVectors();
	m_fov = fov;
	m_tagetMode = eTarget_none;
	//m_nextTranslation = m_translation;
}

Camera::Camera(TiXmlElement* node, ZobObject* parent)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, node, NULL, parent)
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
		return;
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
	engine->QueueLine(camera, &v0, &v1, c, true);
	engine->QueueLine(camera, &v1, &v3, c, true);
	engine->QueueLine(camera, &v2, &v3, c2, true);
	engine->QueueLine(camera, &v2, &v0, c, true);
	engine->QueueLine(camera, p, &v1, c, true);
	engine->QueueLine(camera, p, &v2, c, true);
	engine->QueueLine(camera, p, &v3, c, true);
	engine->QueueLine(camera, p, &v0, c, true);
}

void Camera::Zoom(float z)
{
	ZobVector3 v = m_forward;
	v = v * (-z);
	//m_nextTranslation = m_nextTranslation + v;
	v = v + GetPosition();
	SetPosition(v.x, v.y, v.z);
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

void Camera::RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle)
{
	//g_update_camera_mutex.lock();
	//ZobVector3 t = m_nextTranslation;
	ZobVector3 t = GetPosition();
	t.x -= point->x;
	t.y -= point->y;
	t.z -= point->z;
	ZobMatrix4x4 rot = ZobMatrix4x4::RotateAroundAxis(axis, angle);
	rot.Mul(&t);
	ZobVector3 tn = t;
	tn.Normalize();
	t.x += point->x;
	t.y += point->y;
	t.z += point->z;
	if (lockAxis)
	{
		float f = ZobVector3::Dot(&tn, lockAxis);
		if (fabs(f) > 0.99)
		{
			return;
		}
	}
	//m_nextTranslation = t;
	//g_update_camera_mutex.unlock();
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
	}
}

void Camera::Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix)
{
	g_update_camera_mutex.lock();
	//m_translation = m_nextTranslation;
	m_left = ZobVector3(1, 0, 0);
	//m_forward = ZobVector3(0, 0, 1);
	m_up = ZobVector3(0, 1, 0);
	if (m_tagetMode != eTarget_none)
	{
		const ZobVector3* p = GetPosition();
		if (m_tagetMode == eTarget_Vector &&  m_targetVector != p)
		{
			ZobVector3 v = m_targetVector - GetPosition();
			v.Normalize();
			v = ZobVector3::GetAnglesFromVector(v);
			SetRotation(v.x, v.y, v.z);

		}
		else if (m_tagetMode == eTarget_Object && m_targetObject)
		{
			const ZobVector3* tp = m_targetObject->GetPosition();
			ZobVector3 v = ZobVector3(tp);
			if( v != GetPosition())
			{
				v = ZobVector3(tp);
				v = v - p;
				v.Normalize();
				//m_forward = v;
				v = ZobVector3::GetAnglesFromVector(v);
				SetRotation(v.x, v.y, v.z);
			}
		}
	}
	//m_nextTranslation = m_translation;
	ZobObject::Update(parentMatrix, parentRSMatrix);
	m_rotationScaleMatrix.Identity();
	m_rotationScaleMatrix.SetRotation(GetRotation());
	/*
	m_left = ZobVector3(1, 0, 0);
	m_forward = ZobVector3(0, 0, 1);
	m_up = ZobVector3(0, 1, 0);
	m_rotationScaleMatrix.Mul(&m_left);
	m_rotationScaleMatrix.Mul(&m_forward);
	m_rotationScaleMatrix.Mul(&m_up);
	*/
	g_update_camera_mutex.unlock();
}

void Camera::UpdateViewProjectionMatrix()
{
	g_update_camera_mutex.lock();
	m_viewTransaltion = GetPosition();
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);
	ZobVector3 p = m_viewTransaltion;
	ZobVector3 left = ZobVector3(1, 0, 0);
	ZobVector3 forward = ZobVector3(0, 0, 1);
	ZobVector3 up = ZobVector3(0, 1, 0);
	ZobMatrix4x4 m_invRotationMatrix;
	ZobMatrix4x4::InvertMatrix4(m_rotationScaleMatrix, m_invRotationMatrix);
	m_invRotationMatrix.Mul(&left);
	m_invRotationMatrix.Mul(&forward);
	m_invRotationMatrix.Mul(&up);
	SetViewMatrix(&left, &up, &forward, &p);
	m_forward = forward;
	m_up = up;
	m_left = left;
	g_update_camera_mutex.unlock();
}

void Camera::SetViewMatrix(const ZobVector3& left, const ZobVector3& up, const ZobVector3& fw, const ZobVector3& p)
{
	m_viewRotMatrix.SetData(0, 0, left.x);
	m_viewRotMatrix.SetData(0, 1, up.x);
	m_viewRotMatrix.SetData(0, 2, fw.x);
	m_viewRotMatrix.SetData(0, 3, 0);//-p.x);

	m_viewRotMatrix.SetData(1, 0, left.y);
	m_viewRotMatrix.SetData(1, 1, up.y);
	m_viewRotMatrix.SetData(1, 2, fw.y);
	m_viewRotMatrix.SetData(1, 3, 0);//-p.y);

	m_viewRotMatrix.SetData(2, 0, left.z);
	m_viewRotMatrix.SetData(2, 1, up.z);
	m_viewRotMatrix.SetData(2, 2, fw.z);
	m_viewRotMatrix.SetData(2, 3, 0);//-p.z);

	m_viewRotMatrix.SetData(3, 0, 0);
	m_viewRotMatrix.SetData(3, 1, 0);
	m_viewRotMatrix.SetData(3, 2, 0);
	m_viewRotMatrix.SetData(3, 3, 1);
}

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float zNear, const float zFar)
{
	const float ar = width / height;
	const float zRange = zNear - zFar;
	const float tanHalfFOV = -tanf(angleOfView / 2.0 * M_PI / 180.0);

	m_projMatrix.SetData(0, 0, 1.0f / (tanHalfFOV * ar));
	m_projMatrix.SetData(0, 1, 0.0f);
	m_projMatrix.SetData(0, 2, 0.0f);
	m_projMatrix.SetData(0, 3, 0.0f);

	m_projMatrix.SetData(1, 0, 0.0f);
	m_projMatrix.SetData(1, 1, 1.0f / tanHalfFOV);
	m_projMatrix.SetData(1, 2, 0.0f);
	m_projMatrix.SetData(1, 3, 0.0f);

	m_projMatrix.SetData(2, 0, 0.0f);
	m_projMatrix.SetData(2, 1, 0.0f);
	m_projMatrix.SetData(2, 2, (-zNear - zFar) / zRange);
	m_projMatrix.SetData(2, 3, 2.0f * zFar * zNear / zRange);

	m_projMatrix.SetData(3, 0, 0.0f);
	m_projMatrix.SetData(3, 1, 0.0f);
	m_projMatrix.SetData(3, 2, 1.0f);
	m_projMatrix.SetData(3, 3, 0.0f);
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
