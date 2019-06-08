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

void Camera::setProjectionMatrix(const float angleOfView, const float near, const float far)
{
	float scale = 1 / tan(angleOfView * 0.5 * M_PI / 180);
	m_projMatrix.SetData(0, 0, scale); // scale the x coordinates of the projected point
	m_projMatrix.SetData(1, 1, scale); // scale the y coordinates of the projected point
	m_projMatrix.SetData(2, 2, -far / (far - near)); // used to remap z to (0,1]
	m_projMatrix.SetData(3, 2, -far * near / (far - near)); // used to remap z (0,1]
	m_projMatrix.SetData(2, 3, -1); // set w = -z
	m_projMatrix.SetData(3, 3, 0);
}