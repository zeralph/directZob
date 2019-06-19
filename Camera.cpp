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
	m_projMatrix.SetData(0, 0, 2 / (r - l));
	m_projMatrix.SetData(0, 1, 0); 
	m_projMatrix.SetData(0, 2, 0);
	m_projMatrix.SetData(0, 3, -1 / (r - l));

	m_projMatrix.SetData(1, 0, 0);
	m_projMatrix.SetData(1, 1, 2 / (t - b));
	m_projMatrix.SetData(1, 2, 0);
	m_projMatrix.SetData(1, 3, -(t + b) / (t - b));

	m_projMatrix.SetData(2, 0, 0);
	m_projMatrix.SetData(2, 1, 0);
	m_projMatrix.SetData(2, 2, -2 / (f - n));
	m_projMatrix.SetData(2, 3, -(f + n) / (f - n));

	m_projMatrix.SetData(3, 0, 0);
	m_projMatrix.SetData(3, 1, 0);
	m_projMatrix.SetData(3, 2, 0);
	m_projMatrix.SetData(3, 3, 1);


}
