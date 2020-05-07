#include "Camera.h"
#include "DirectZob.h"
#include <math.h>



Camera::Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, name, NULL, parent)
{
	//RecomputeVectors();
	m_fov = fov;
}

Camera::~Camera()
{
}

void Camera::Draw(const Camera* camera, Core::Engine* engine)
{
	if (GetType() == ZOBGUID::type_editor)
	{
		return;
	}
	Vector3 v0 = Vector3(-2, 1, 1);
	Vector3 v1 = Vector3(2, 1, 1);
	Vector3 v2 = Vector3(-2, -1, 1);
	Vector3 v3 = Vector3(2, -1, 1);
	m_rotationScaleMatrix.Mul(&v0);
	m_rotationScaleMatrix.Mul(&v1);
	m_rotationScaleMatrix.Mul(&v2);
	m_rotationScaleMatrix.Mul(&v3);
	v0 = v0 + m_translation;
	v1 = v1 + m_translation;
	v2 = v2 + m_translation;
	v3 = v3 + m_translation;
	uint c = 0x000000FF;
	uint c2 = 0x0000FFFF;
	engine->QueueLine(camera, &v0, &v1, c, true);
	engine->QueueLine(camera, &v1, &v3, c, true);
	engine->QueueLine(camera, &v2, &v3, c2, true);
	engine->QueueLine(camera, &v2, &v0, c, true);
	engine->QueueLine(camera, &m_translation, &v1, c, true);
	engine->QueueLine(camera, &m_translation, &v2, c, true);
	engine->QueueLine(camera, &m_translation, &v3, c, true);
	engine->QueueLine(camera, &m_translation, &v0, c, true);
}

void Camera::Zoom(float z)
{
	Vector3 v = GetForward();
	v = v * (-z);
	m_translation = m_translation + v;
}

void Camera::RotateAroundAxis(const Vector3* axis, float dx, float dy)
{
	/*
	Vector3 l = Vector3::RotateAroundAxis(axis, Vector3::Vector3Y, -dx * M_PI / 180.0);
	//l = Vector3::RotateAroundAxis(l, Vector3::Vector3Y, -dx * M_PI / 180.0);
	m_translation = l + axis;
	RecomputeVectors();
	if (m_cameraFw.y >= 0.95f && dy > 0)
	{
		dy = 0.0f;
	}
	if (m_cameraFw.y <= -0.95f && dy < 0)
	{
		dy = 0.0f;
	}
	l = m_translation - axis;
	l = Vector3::RotateAroundAxis(l, m_cameraLeft, dy * M_PI / 180.0);
	m_translation = l + axis;
	RecomputeVectors();
	*/
}

void Camera::Move(float dx, float dy)
{
	/*
	Vector3 vl = Vector3(m_cameraLeft);
	vl = vl * ((float)-dx / 20.0f);
	vl.y = 0;

	Vector3 vf = Vector3(m_cameraFw);
	
	vf = vf * ((float)dy / 20.0f);
	vf.y = 0;
	m_translation = m_translation - (vl + vf);
//	m_cameraTarget = m_cameraTarget - (vl + vf);
*/
}

void Camera::Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix)
{
	static bool gg = false;
	if (gg)
	{
		Vector3 v = Vector3(0, 0, 0);
		SetTarget(&v);
	}
	ZobObject::Update(parentMatrix, parentRSMatrix);
	//Vector3 x = Vector3(0, 0, 0);
	//SetTarget(&x);
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);
	Vector3 p = m_translation;
	m_left = Vector3(1, 0, 0);
	m_forward = Vector3(0, 0, 1);
	m_up = Vector3(0,1, 0);
	Matrix4x4 m_invRotationMatrix;
	Matrix4x4::InvertMatrix4(m_rotationScaleMatrix, m_invRotationMatrix);
	m_invRotationMatrix.Mul(&m_left);
	m_invRotationMatrix.Mul(&m_forward);
	m_invRotationMatrix.Mul(&m_up);
	SetViewMatrix(&m_left, &m_up, &m_forward, &p);
}


void Camera::SetViewMatrix(const Vector3& left, const Vector3& up, const Vector3& fw, const Vector3& p)
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

bool Camera::SetTarget(const Vector3* t)
{
	if (t != &m_translation)
	{
		Vector3 v = t - m_translation;
		v.Normalize();
		float pitch = asin(-v.y);
		float yaw = atan2(v.x, v.z);
		m_rotation.x = -pitch * 180.0f / M_PI;
		m_rotation.y = -yaw * 180.0f / M_PI;
		m_rotation.z = 0.0f * v.z;
		return true;
	}
	return false;
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