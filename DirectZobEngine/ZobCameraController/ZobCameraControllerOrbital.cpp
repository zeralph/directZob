#include "ZobCameraControllerOrbital.h"
#include "../Managers/ZobInputManager.h"
#include "../DirectZob.h"

ZobCameraControllerOrbital::ZobCameraControllerOrbital(Camera* c, bool isFree, std::string guid) :ZobCameraController(c, guid)
{
	if (isFree)
	{
		m_type = Camera::eCamera_orbital_free;
	}
	else
	{
		m_type = Camera::eCamera_orbital;
	}
	m_centerOfRotation = ZobVector3(0, 0, 0);
	m_isFree = isFree;
	m_rotationX = -45.0f;
	m_rotationY = -45.0f;
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
	m_dist = 40.0f;
}

ZobCameraControllerOrbital::ZobCameraControllerOrbital(Camera* c, bool isFree) :ZobCameraController(c)
{
	if (isFree)
	{
		m_type = Camera::eCamera_orbital_free;
	}
	else
	{
		m_type = Camera::eCamera_orbital;
	}
	m_centerOfRotation = ZobVector3(0, 0, 0);
	m_isFree = isFree;
	m_rotationX = -45.0f;
	m_rotationY = -45.0f;
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
	m_dist = 40.0f;
}

ZobCameraControllerOrbital::~ZobCameraControllerOrbital()
{
    m_zobCamera = NULL;
}

void ZobCameraControllerOrbital::PreUpdate(float dt)
{
	if (m_zobCamera->m_active)
	{
		const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
		float rx = -inputMap->GetFloat(ZobInputManager::RightStickX) * 2.0f;
		float ry = -inputMap->GetFloat(ZobInputManager::RightStickY) * 2.0f;
		Rotate(rx, -ry, 0);
		float mx = inputMap->GetFloat(ZobInputManager::LeftStickX) * 2.0f;
		float my = inputMap->GetFloat(ZobInputManager::LeftStickY) * 2.0f;
		//Move(-mx, 0, -my);
	}
}

void ZobCameraControllerOrbital::Update(float dt)
{
	if (m_isFree)
	{
		ZobVector3 vl = m_zobCamera->m_left;
		vl = vl * (-m_moveX / 20.0f);
		vl.y = 0;
		ZobVector3 vf = m_zobCamera->m_forward;
		vf = vf * (m_moveZ / 20.0f);
		vf.y = 0;
		ZobVector3 vu = ZobVector3(0, m_moveY / 20.0f, 0);
		ZobVector3 vp = m_zobCamera->GetWorldPosition();
		ZobVector3 deltaP = (vl + vf + vu);
		vp = vp - deltaP;
		m_centerOfRotation = m_centerOfRotation - deltaP;
		m_zobCamera->SetWorldPosition(vp.x, vp.y, vp.z);
		m_moveX = 0.0f;
		m_moveY = 0.0f;
		m_moveZ = 0.0f;
	}
	else
	{
		m_centerOfRotation = m_zobCamera->GetParent()->GetWorldPosition();
	}
	m_rotationY = std::fminf(std::fmaxf(m_rotationY, -89.9f), 89.9f);
	m_zobCamera->RotateOrbital(&m_centerOfRotation, m_rotationX, m_rotationY, m_dist);
}

void ZobCameraControllerOrbital::Rotate(float x, float y, float z)
{
	m_rotationX += x;
	m_rotationY += y;
}

void ZobCameraControllerOrbital::Move(float x, float y, float z)
{
	m_moveX += x;
	m_moveY += y;
	m_moveZ += z;
}

void ZobCameraControllerOrbital::Zoom(float f)
{
	m_dist += f;
	m_dist = fmaxf(0.25f, m_dist);
}