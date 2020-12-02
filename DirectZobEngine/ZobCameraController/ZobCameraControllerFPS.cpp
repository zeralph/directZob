#include "ZobCameraControllerFPS.h"
#include "../ZobPhysic/ZobPhysicComponent.h"

ZobCameraControllerFPS::ZobCameraControllerFPS(Camera* c) :ZobCameraController(c)
{
	m_type = Camera::eCamera_fps;
	m_pitch = 0.0;
	m_yaw = 0.0f;
}

ZobCameraControllerFPS::~ZobCameraControllerFPS()
{
    m_zobCamera = NULL;
}

void ZobCameraControllerFPS::Update()
{
	ZobVector3 v = m_zobCamera->GetWorldPosition();
	if (m_pitch < DEG_TO_RAD(-90.0f))
	{
		m_pitch = DEG_TO_RAD(-90.0f);
	}
	if (m_pitch > DEG_TO_RAD(90.0f))
	{
		m_pitch = DEG_TO_RAD(90.0f);
	}
	ZobVector3 zqNew = ZobMatrix4x4::EulerToQuaternion(m_pitch, m_yaw, 0);
	Quaternion qNew = Quaternion(zqNew.x, zqNew.y, zqNew.z, zqNew.w);
	Vector3 vz = Vector3(0,0,0);
	Transform tNew = Transform(vz, qNew);
	Transform newTransform = m_zobCamera->m_physicComponent->GetWorldTransform();
	tNew = newTransform * tNew;
	//Quaternion::normalize(tNew);
	
	Vector3 left = tNew.getOrientation() * Vector3(1, 0, 0);
	Vector3 up = tNew.getOrientation() * Vector3(0, 1, 0);
	Vector3 forward = tNew.getOrientation() * Vector3(0, 0, 1);
	m_zobCamera->m_physicComponent->SetWorldTransform(tNew);
	m_zobCamera->m_left.x = left.x;
	m_zobCamera->m_left.y = left.y;
	m_zobCamera->m_left.z = left.z;
	m_zobCamera->m_up.x = up.x;
	m_zobCamera->m_up.y = up.y;
	m_zobCamera->m_up.z = up.z;
	m_zobCamera->m_forward.x = forward.x;
	m_zobCamera->m_forward.y = forward.y;
	m_zobCamera->m_forward.z = forward.z;
	m_zobCamera->UpdateViewProjectionMatrix(&v);

	m_zobCamera->m_rotationScaleMatrix.Mul(&m_zobCamera->m_left);
	m_zobCamera->m_rotationScaleMatrix.Mul(&m_zobCamera->m_forward);
	m_zobCamera->m_rotationScaleMatrix.Mul(&m_zobCamera->m_up);
	//m_zobCamera->LookAt(&m_zobCamera->m_forward, &m_zobCamera->m_left, &m_zobCamera->m_up, false);


	//m_pitch = 0.0f;
	//m_yaw = 0.0f;
}

void ZobCameraControllerFPS::Rotate(float x, float y, float z)
{
	m_pitch += y/20.0f;
	m_yaw += x/20.0f;
}

void ZobCameraControllerFPS::Move(float x, float y, float z)
{
	ZobVector3 v = m_zobCamera->GetWorldPosition();
	ZobVector3 le = m_zobCamera->GetLeft() * x;
	ZobVector3 up = m_zobCamera->GetUp() * y;
	ZobVector3 fw = m_zobCamera->GetForward() * z;
	v.x += le.x + up.x + fw.x;
	v.y += le.y + up.y + fw.y;
	v.z += le.z + up.z + fw.z;
	m_zobCamera->SetWorldPosition(v.x, v.y, v.z);
}
