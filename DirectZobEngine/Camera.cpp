#include "Camera.h"
#include "DirectZob.h"
#include <math.h>



Camera::Camera(const std::string& name, Vector3& position, Vector3& target, Vector3& up, float fov, BufferData* bufferData)
	:ZobObject(ZOBGUID::Type::type_scene, ZOBGUID::SubType::subtype_zobCamera, name, NULL, NULL)
{
	m_name = name.c_str();
	m_translation = position;
//	m_cameraPosition = position;
	m_cameraTarget = target;
	m_cameraUp = up;
	RecomputeVectors();
	m_fov = fov;
//	m_bufferData = bufferData;
	m_projMatrix.Identity();
	m_viewMatrix.Identity();
}

Camera::~Camera()
{
}

void Camera::Zoom(float z)
{
	Vector3 v = m_cameraFw;
	v = v * (-z);
	m_translation = m_translation + v;
}

void Camera::RotateAroundAxis(float dx, float dy)
{
	Vector3 l = m_translation - m_cameraTarget;
	l = Vector3::RotateAroundAxis(l, Vector3::Vector3Y, -dx * M_PI / 180.0);
	m_translation = l + m_cameraTarget;
	RecomputeVectors();
	if (m_cameraFw.y >= 0.95f && dy > 0)
	{
		dy = 0.0f;
	}
	if (m_cameraFw.y <= -0.95f && dy < 0)
	{
		dy = 0.0f;
	}
	l = m_translation - m_cameraTarget;
	l = Vector3::RotateAroundAxis(l, m_cameraLeft, dy * M_PI / 180.0);
	m_translation = l + m_cameraTarget;
	RecomputeVectors();
}

void Camera::Move(float dx, float dy)
{
	Vector3 vl = Vector3(m_cameraLeft);
	vl = vl * ((float)-dx / 20.0f);
	vl.y = 0;

	Vector3 vf = Vector3(m_cameraFw);
	
	vf = vf * ((float)dy / 20.0f);
	vf.y = 0;
	m_translation = m_translation - (vl + vf);
	m_cameraTarget = m_cameraTarget - (vl + vf);
}

void Camera::Update()
{
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);
	InitView();
	UpdateLookAt();
	RecomputeVectors();
	m_modelMatrix = m_viewMatrix;
	m_rotation = m_modelMatrix.GetRotation();
}

void Camera::RecomputeVectors()
{
	m_cameraFw = m_translation - m_cameraTarget;
	m_cameraFw.Normalize();
	m_cameraLeft = Vector3::Cross(&m_cameraFw, &m_cameraUp);
	m_cameraLeft.Normalize();
}

void Camera::SetLookAt(const Vector3* from, const Vector3* to, const Vector3* up)
{
	m_translation = from;
	m_cameraTarget = to;
	m_cameraUp = up;
	m_translation.Mul(-1.0f);
	m_cameraTarget.Mul(-1.0f);
	//m_cameraUp.Mul(-1.0f);
}

void Camera::UpdateLookAt()
{
	//Vector3 zaxis = Vector3(eye->x - at->x, eye->y - at->y, eye->z - at->z);
	const Vector3* eye = &m_translation;
	const Vector3* at = &m_cameraTarget;
	const Vector3* up = &m_cameraUp;
	Vector3 zaxis = Vector3(at->x - eye->x, at->y - eye->y, at->z - eye->z);
	
	zaxis.Normalize();
	Vector3 xaxis = Vector3::Cross(&zaxis, up);
	xaxis.Normalize();
	
	Vector3 yaxis = Vector3::Cross(&xaxis, &zaxis);
	zaxis.x = -zaxis.x;
	zaxis.y = -zaxis.y;
	zaxis.z = -zaxis.z;

	m_viewMatrix.SetData(0, 0, xaxis.x);
	m_viewMatrix.SetData(0, 1, xaxis.y);
	m_viewMatrix.SetData(0, 2, xaxis.z);
	m_viewMatrix.SetData(0, 3, Vector3::Dot(&xaxis, eye));

	m_viewMatrix.SetData(1, 0, yaxis.x);
	m_viewMatrix.SetData(1, 1, yaxis.y);
	m_viewMatrix.SetData(1, 2, yaxis.z);
	m_viewMatrix.SetData(1, 3, Vector3::Dot(&yaxis, eye));

	m_viewMatrix.SetData(2, 0, zaxis.x);
	m_viewMatrix.SetData(2, 1, zaxis.y);
	m_viewMatrix.SetData(2, 2, zaxis.z);
	m_viewMatrix.SetData(2, 3, Vector3::Dot(&zaxis, eye));

	m_viewMatrix.SetData(3, 0, 0);
	m_viewMatrix.SetData(3, 1, 0);
	m_viewMatrix.SetData(3, 2, 0);
	m_viewMatrix.SetData(3, 3, 1);
}

/*
void Camera::SetLookAt(const Vector3* from, const Vector3* to, const Vector3* tmp)
{
	Vector3 forward = Vector3(to->x - from->x, to->y - from->y, to->z - from->z);
	forward.Normalize();
	Vector3 right = Vector3::Cross(tmp, &forward);
	right.Normalize();
	Vector3 up = Vector3::Cross(&forward, &right);
	up.Normalize();
	m_viewMatrix.Identity();
	static bool b = true;
	if (b)
	{
		m_viewMatrix.SetData(0, 0, right.x);
		m_viewMatrix.SetData(0, 1, right.y);
		m_viewMatrix.SetData(0, 2, right.z);
		//m_viewMatrix.SetData(0, 3, -from->x);

		m_viewMatrix.SetData(1, 0, up.x);
		m_viewMatrix.SetData(1, 1, up.y);
		m_viewMatrix.SetData(1, 2, up.z);
		//m_viewMatrix.SetData(1, 3, -from->y);

		m_viewMatrix.SetData(2, 0, forward.x);
		m_viewMatrix.SetData(2, 1, forward.y);
		m_viewMatrix.SetData(2, 2, forward.z);
		//m_viewMatrix.SetData(2, 3, -from->z);

		m_viewMatrix.SetData(0, 3, -right.x * from->x - right.y * from->y - right.z * from->z);
		m_viewMatrix.SetData(1, 3, -up.x * from->x - up.y * from->y - up.z * from->z);
		m_viewMatrix.SetData(2, 3, -forward.x * from->x - forward.y * from->y - forward.z * from->z);
	}
	else
	{
		m_viewMatrix.SetData(0, 0, 1);
		m_viewMatrix.SetData(0, 1, 0);
		m_viewMatrix.SetData(0, 2, 0);
		m_viewMatrix.SetData(0, 3, 0);

		m_viewMatrix.SetData(1, 0, 0);
		m_viewMatrix.SetData(1, 1, 1);
		m_viewMatrix.SetData(1, 2, 0);
		m_viewMatrix.SetData(1, 3, 0);

		m_viewMatrix.SetData(2, 0, 0);
		m_viewMatrix.SetData(2, 1, 0);
		m_viewMatrix.SetData(2, 2, 1);
		m_viewMatrix.SetData(2, 3, 0);

		m_viewMatrix.SetData(3, 0, 0);
		m_viewMatrix.SetData(3, 1, 0);
		m_viewMatrix.SetData(3, 2, 0);
		m_viewMatrix.SetData(3, 3, 1);
	}
}
*/
void Camera::InitView()
{
	m_viewMatrix.SetData(0, 0, 1);
	m_viewMatrix.SetData(0, 1, 0);
	m_viewMatrix.SetData(0, 2, 0);
	m_viewMatrix.SetData(0, 3, 0);

	m_viewMatrix.SetData(1, 0, 0);
	m_viewMatrix.SetData(1, 1, 1);
	m_viewMatrix.SetData(1, 2, 0);
	m_viewMatrix.SetData(1, 3, 0);

	m_viewMatrix.SetData(2, 0, 0);
	m_viewMatrix.SetData(2, 1, 0);
	m_viewMatrix.SetData(2, 2, 1);
	m_viewMatrix.SetData(2, 3, 0);

	m_viewMatrix.SetData(3, 0, 0);
	m_viewMatrix.SetData(3, 1, 0);
	m_viewMatrix.SetData(3, 2, 0);
	m_viewMatrix.SetData(3, 3, 1);
};

/*
void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	float n, f, t, r,w, h, imageAspectRatio, scale;
	n = near;
	f = far;
	w = width / 2.0f;
	h = height / 2.0f;
	imageAspectRatio = w / h;
	scale = -tan(angleOfView * 0.5 * M_PI / 180.0) * n;
	r = imageAspectRatio * scale;
	t = scale;

//	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	m_projMatrix.Identity();
	m_projMatrix.SetData(0, 0, n/r);
	m_projMatrix.SetData(0, 1, 0); 
	m_projMatrix.SetData(0, 2, 0);
	m_projMatrix.SetData(0, 3, 0);

	m_projMatrix.SetData(1, 0, 0);
	m_projMatrix.SetData(1, 1, n/t);
	m_projMatrix.SetData(1, 2, 0);
	m_projMatrix.SetData(1, 3, 0);

	m_projMatrix.SetData(2, 0, 0);
	m_projMatrix.SetData(2, 1, 0);
	m_projMatrix.SetData(2, 2, -(f + n) / (f - n));
	m_projMatrix.SetData(2, 3, -(2 * f * n) / (f - n));

	m_projMatrix.SetData(3, 0, 0);
	m_projMatrix.SetData(3, 1, 0);
	m_projMatrix.SetData(3, 2, -1);
	m_projMatrix.SetData(3, 3, 0);


}
*/

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