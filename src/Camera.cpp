#include "Camera.h"
#include <math.h>



Camera::Camera()
{
	m_projMatrix.Identity();
	m_viewMatrix.Identity();
}

Camera::~Camera()
{

}

void Camera::SetLookAt(const Vector3* eye, const Vector3* at, const Vector3* up)
{
	Vector3 zaxis = Vector3(eye->x - at->x, eye->y - at->y, eye->z - at->z);
	zaxis.Normalize();
	Vector3 xaxis = Vector3::Cross(&zaxis, up);
	xaxis.Normalize();
	Vector3 yaxis = Vector3::Cross(&xaxis, &zaxis);

	zaxis.x = -zaxis.x;
	zaxis.y = -zaxis.y;
	zaxis.z = -zaxis.z;
//	zaxis.w = -zaxis.w;

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

	m_cameraPosition.x = m_viewMatrix.GetValue(0, 3);
	m_cameraPosition.y = m_viewMatrix.GetValue(1, 3);
	m_cameraPosition.z = m_viewMatrix.GetValue(2, 3);
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

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	const float ar = width / height;
	const float zRange = near - far;
	const float tanHalfFOV = tanf(angleOfView / 2.0 * M_PI / 180.0);

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
	m_projMatrix.SetData(2, 2, (-near - far) / zRange);
	m_projMatrix.SetData(2, 3, 2.0f * far * near / zRange);

	m_projMatrix.SetData(3, 0, 0.0f);
	m_projMatrix.SetData(3, 1, 0.0f);
	m_projMatrix.SetData(3, 2, 1.0f);
	m_projMatrix.SetData(3, 3, 0.0f);
}