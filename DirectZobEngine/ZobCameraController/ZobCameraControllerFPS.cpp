#include "ZobCameraControllerFPS.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../DirectZob.h"

ZobCameraControllerFPS::ZobCameraControllerFPS(Camera* c) :ZobCameraController(c)
{
	m_type = Camera::eCamera_fps;
	m_deltaPitch = 0.0f;
	m_deltaYaw = 0.0f;
	m_deltaRoll = 0.0f;
	m_totalPitch = 0.0f;
	m_totalYaw = 0.0f;
	m_totalRoll = 0.0f;
	m_moveX	= 0.0f;
	m_moveY	= 0.0f;
	m_moveZ	= 0.0f;
	m_typeName = "FPSCamera";
}

ZobCameraControllerFPS::~ZobCameraControllerFPS()
{
    m_zobCamera = NULL;
}

void ZobCameraControllerFPS::PreUpdate()
{
	if (m_zobCamera->m_active)
	{
		const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
		float rx = inputMap->GetFloat(ZobInputManager::RightStickX) * 2.0f;
		float ry = inputMap->GetFloat(ZobInputManager::RightStickY) * 2.0f;
		Rotate(-rx, -ry, 0);
		float mx = inputMap->GetFloat(ZobInputManager::LeftStickX) / 2.0f;
		float my = inputMap->GetFloat(ZobInputManager::LeftStickY) / 2.0f;
		Move(-mx, 0, my);
	}
}

void ZobCameraControllerFPS::Update(float dt)
{
	m_totalPitch += m_deltaPitch;
	m_totalRoll += m_deltaRoll;
	m_totalYaw += m_deltaYaw;
	//move
	ZobVector3 v = m_zobCamera->GetWorldPosition();
	ZobVector3 le = m_zobCamera->GetLeft() * m_moveX;
	ZobVector3 up = m_zobCamera->GetUp() * m_moveY;
	ZobVector3 fw = m_zobCamera->GetForward() * m_moveZ;
	v.x += le.x + up.x + fw.x;
	v.y += le.y + up.y + fw.y;
	v.z += le.z + up.z + fw.z;
	m_zobCamera->SetWorldPosition(v.x, v.y, v.z);
	//rotation
	if (m_totalPitch < DEG_TO_RAD(-90.0f))
	{
		m_totalPitch = DEG_TO_RAD(-90.0f);
		m_deltaPitch = 0.0f;
	}
	if (m_totalPitch > DEG_TO_RAD(90.0f))
	{
		m_totalPitch = DEG_TO_RAD(90.0f);
		m_deltaPitch = 0.0f;
	}
	ZobVector3 deltaZ1 = ZobMatrix4x4::EulerToQuaternion(m_deltaPitch, m_deltaYaw, m_deltaRoll);
	Quaternion deltaQ1 = Quaternion(deltaZ1.x, deltaZ1.y, deltaZ1.z, deltaZ1.w);
	Quaternion qAct = m_zobCamera->m_physicComponent->GetLocalTransform().getOrientation();
	qAct.normalize();
	Quaternion qFinal;
	qFinal = qAct * deltaQ1;
	qFinal.normalize();
	if (qFinal.isValid())
	{
		Vector3 left = qFinal * Vector3(1, 0, 0);
		Vector3 up = qFinal * Vector3(0, 1, 0);
		Vector3 forward = qFinal * Vector3(0, 0, 1);
		left.y = 0;
		left.normalize();
		forward = left.cross(up);
		forward.normalize();
		up = forward.cross(left);
		forward.normalize();
		m_zobCamera->m_left.x = left.x;
		m_zobCamera->m_left.y = left.y;
		m_zobCamera->m_left.z = left.z;
		m_zobCamera->m_up.x = up.x;
		m_zobCamera->m_up.y = up.y;
		m_zobCamera->m_up.z = up.z;
		m_zobCamera->m_forward.x = forward.x;
		m_zobCamera->m_forward.y = forward.y;
		m_zobCamera->m_forward.z = forward.z;
		m_zobCamera->LookAt(&m_zobCamera->m_forward, &m_zobCamera->m_left, &m_zobCamera->m_up, false);
		//m_zobCamera->UpdateViewProjectionMatrix(&v);
		m_deltaPitch = 0.0f;
		m_deltaYaw = 0.0f;
		m_deltaRoll = 0.0f;
	}
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
}

void ZobCameraControllerFPS::Rotate(float x, float y, float z)
{
	//return;
	m_deltaPitch += DEG_TO_RAD(y);
	m_deltaYaw += DEG_TO_RAD(x);
	m_deltaRoll += DEG_TO_RAD(0.0f);
}

void ZobCameraControllerFPS::Move(float x, float y, float z)
{
	m_moveX += x;
	m_moveY += y;
	m_moveZ += z;
}
