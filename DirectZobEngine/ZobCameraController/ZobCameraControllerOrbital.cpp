#include "ZobCameraControllerOrbital.h"
#include "../Managers/ZobInputManager.h"
#include "../DirectZob.h"

ZobCameraControllerOrbital::ZobCameraControllerOrbital(Camera* c) :ZobCameraController(c)
{
	m_type = Camera::eCamera_orbital;
	m_typeName = "FreeOrbitalCamera";
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
}

ZobCameraControllerOrbital::~ZobCameraControllerOrbital()
{
    m_zobCamera = NULL;
}

void ZobCameraControllerOrbital::PreUpdate()
{
	if (m_zobCamera->m_active)
	{
		const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
		float rx = inputMap->GetFloat(ZobInputManager::RightStickX) * 2.0f;
		float ry = inputMap->GetFloat(ZobInputManager::RightStickY) * 2.0f;
		Rotate(rx, -ry, 0);
		float mx = inputMap->GetFloat(ZobInputManager::LeftStickX) * 2.0f;
		float my = inputMap->GetFloat(ZobInputManager::LeftStickY) * 2.0f;
		Move(-mx, 0, -my);
	}
}

void ZobCameraControllerOrbital::Update()
{
	//rotation section
	ZobVector3 v = ZobVector3::Vector3Y;
	ZobVector3 p = ZobVector3::Vector3Zero;
	m_zobCamera->GetTargetVector(&p);
	m_zobCamera->RotateAroundPointAxis(&p, &v, NULL, m_rotationX, false);
	v = m_zobCamera->GetLeft();
	m_zobCamera->RotateAroundPointAxis(&p, &v, NULL, m_rotationY, false);
	//move section
	ZobVector3 vl = ZobVector3(m_zobCamera->m_left);
	vl = vl * (-m_moveX / 20.0f);
	vl.y = 0;
	ZobVector3 vf = ZobVector3(m_zobCamera->m_forward);
	vf = vf * (m_moveZ / 20.0f);
	vf.y = 0;
	ZobVector3 vu = ZobVector3(0, m_moveY / 20.0f, 0);
	ZobVector3 vp = m_zobCamera->GetWorldPosition();
	vp = vp - (vl + vf + vu);
	m_zobCamera->SetWorldPosition(vp.x, vp.y, vp.z);
	if (true)
	{
		m_zobCamera->m_targetVector = m_zobCamera->m_targetVector - (vl + vf + vu);
	}
	//Update section
	ZobVector3 worldPos = m_zobCamera->GetWorldPosition();
	if (m_zobCamera->m_tagetMode == Camera::eTarget_Vector && m_zobCamera->m_targetVector != worldPos)
	{
		ZobVector3 up = ZobVector3::Vector3Y;
		ZobVector3 forward = &m_zobCamera->m_targetVector;
		forward.x -= worldPos.x;
		forward.y -= worldPos.y;
		forward.z -= worldPos.z;
		forward.Normalize();
		ZobVector3 left = ZobVector3::Cross(&up, &forward);
		left.Normalize();
		up = ZobVector3::Cross(&forward, &left);
		up.Normalize();
		m_zobCamera->m_left = left;
		m_zobCamera->m_forward = forward;
		m_zobCamera->m_up = up;
		m_zobCamera->LookAt(&m_zobCamera->m_forward, &m_zobCamera->m_left, &m_zobCamera->m_up, false);
	}
	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
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
