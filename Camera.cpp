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



void Camera::SetLookAt(const Vector3* from, const Vector3* to, const Vector3* tmp)
{
	Vector3 forward = Vector3(to->x - from->x, to->y - from->y, to->z - from->z);
	forward.Normalize();
	Vector3 right = Vector3::Cross(tmp, &forward);
	Vector3 up = Vector3::Cross(&forward, &right);

	m_viewMatrix.Identity();

	m_viewMatrix.SetData(0, 0, right.x);
	m_viewMatrix.SetData(0, 1, right.y);
	m_viewMatrix.SetData(0, 2, right.z);

	m_viewMatrix.SetData(1, 0, up.x);
	m_viewMatrix.SetData(1, 1, up.y);
	m_viewMatrix.SetData(1, 2, up.z);

	m_viewMatrix.SetData(2, 0, forward.x);
	m_viewMatrix.SetData(2, 1, forward.y);
	m_viewMatrix.SetData(2, 2, forward.z);

	m_viewMatrix.SetData(0, 3, -from->x);
	m_viewMatrix.SetData(1, 3, -from->y);
	m_viewMatrix.SetData(2, 3, -from->z);
}


/*
void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	float w = width / 2.0f;
	float h = height / 2.0f;
	float scale =  1.0f / (float)tan(angleOfView * 0.5 * M_PI / 180.0);
	m_projMatrix.Identity();
	m_projMatrix.SetData(0, 0, scale * w); // scale the x coordinates of the projected point
	m_projMatrix.SetData(1, 1, scale * h); // scale the y coordinates of the projected point
	m_projMatrix.SetData(2, 2, -far / (far - near)); // used to remap z to (0,1]
	m_projMatrix.SetData(3, 2, -far * near / (far - near)); // used to remap z (0,1]
	m_projMatrix.SetData(2, 3, -1); // set w = -z
	m_projMatrix.SetData(3, 3, 0);
	m_projMatrix.SetData(0, 3, w);
	m_projMatrix.SetData(1, 3, h);
}
*/
void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	float n, f, b, t, l, r,w, h, imageAspectRatio, scale;
	n = near;
	f = far;
	w = width / 2.0f;
	h = height / 2.0f;
	imageAspectRatio = w / h;
	scale = tan(angleOfView * 0.5 * M_PI / 180) * n;
	r = imageAspectRatio * scale;
	l = -r;
	t = scale;
	b = -t;
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
