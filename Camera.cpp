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

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	float w = width / 2.0f;
	float h = height / 2.0f;
	float scale =  1 / tan(angleOfView * 0.5 * M_PI / 180);
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
