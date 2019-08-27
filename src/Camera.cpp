#include "Camera.h"
#include <math.h>



Camera::Camera(const char* name, Vector3 position, Vector3 target, Vector3 up, float fov, BufferData* bufferData)
{
	m_name = name;
	m_cameraPosition = position;
	m_cameraTarget = target;
	m_cameraUp = up;
	RecomputeVectors();
	m_fov = fov;
	m_bufferData = bufferData;
	m_projMatrix.Identity();
	m_viewMatrix.Identity();
	m_shiftPressed = false;
	m_mouseLeftButtonPressed = false;
	m_mouseRightButtonPressed = false;
	m_mouseX = -1;
	m_mouseY = -1;
}

Camera::~Camera()
{
}

void Camera::OnMouseScroll(float deltaY)
{
	Vector3 v = m_cameraFw;
	v = v * (-deltaY*2.0f);
	m_cameraPosition = m_cameraPosition + v;
}

void Camera::OnMouseButton(MouseButton button, bool isPressed)
{
	if (button == MOUSE_BTN_1)
	{
		if (isPressed)
		{
			m_mouseLeftButtonPressed = true;
		}
		else
		{
			m_mouseLeftButtonPressed = false;
		}
	}
	else if(button == MOUSE_BTN_2)
	{
		if (isPressed)
		{
			m_mouseRightButtonPressed = true;
		}
		else
		{
			m_mouseRightButtonPressed = false;
		}
	}
}

void Camera::OnMouseMove(int x, int y)
{
	float d = 1.0f;
	float r = 1.0f;
	if (m_mouseX < 0)
	{
		m_mouseX = x;
	}
	if (m_mouseY < 0)
	{
		m_mouseY = y;
	}
	int dx = x - m_mouseX;
	int dy = y - m_mouseY;

	if (m_mouseLeftButtonPressed)
	{
		Vector3 vl = Vector3(m_cameraLeft);
		Vector3 vf = Vector3(m_cameraFw);
		vl = vl * ((float)-dx / 20.0f);
		vf = vf * ((float)dy / 20.0f);
		//vf = &Vector3::Vector3Zero;
		vl.y = 0;
		vf.y = 0;
		m_cameraPosition = m_cameraPosition - ( vl + vf );
		m_cameraTarget = m_cameraTarget - (vl + vf);
	}
	if (m_mouseRightButtonPressed)
	{
		Vector3 l = m_cameraPosition - m_cameraTarget;
		l = Vector3::RotateAroundAxis(l, Vector3::Vector3Y, -dx * M_PI / 180.0);
		m_cameraPosition = l + m_cameraTarget;
		RecomputeVectors();
		if (m_cameraFw.y >= 0.95f && dy > 0)
		{
			dy = 0.0f;
		}
		if (m_cameraFw.y <= -0.95f && dy < 0)
		{
			dy = 0.0f;
		}
		l = m_cameraPosition - m_cameraTarget;
		l = Vector3::RotateAroundAxis(l, m_cameraLeft, dy * M_PI / 180.0);
		m_cameraPosition = l + m_cameraTarget;
		RecomputeVectors();
	}
	m_mouseX = x;
	m_mouseY = y;
}

void Camera::OnKeyboardInput(Key key, bool isPressed)
{
	if (key == KB_KEY_RIGHT_SHIFT || key == KB_KEY_LEFT_SHIFT)
	{
		if (isPressed)
		{
			m_shiftPressed = true;
		}
		else
		{
			m_shiftPressed = false;
		}
	}
}

void Camera::Update()
{
	setProjectionMatrix(m_fov, m_bufferData->width, m_bufferData->height, m_bufferData->zNear, m_bufferData->zFar);
	InitView();
	SetLookAt(&m_cameraPosition, &m_cameraTarget, &m_cameraUp);
	RecomputeVectors();
}

void Camera::RecomputeVectors()
{
	m_cameraFw = m_cameraPosition - m_cameraTarget;
	m_cameraFw.Normalize();
	m_cameraLeft = Vector3::Cross(&m_cameraFw, &m_cameraUp);
	m_cameraLeft.Normalize();
}

void Camera::SetLookAt(const Vector3* eye, const Vector3* at, const Vector3* up)
{
	//Vector3 zaxis = Vector3(eye->x - at->x, eye->y - at->y, eye->z - at->z);
	
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

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far)
{
	const float ar = width / height;
	const float zRange = near - far;
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
	m_projMatrix.SetData(2, 2, (-near - far) / zRange);
	m_projMatrix.SetData(2, 3, 2.0f * far * near / zRange);

	m_projMatrix.SetData(3, 0, 0.0f);
	m_projMatrix.SetData(3, 1, 0.0f);
	m_projMatrix.SetData(3, 2, 1.0f);
	m_projMatrix.SetData(3, 3, 0.0f);
}