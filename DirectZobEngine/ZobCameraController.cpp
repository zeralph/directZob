#include "ZobCameraController.h"


ZobCameraController::ZobCameraController(Camera * c)
{
	m_type = Camera::eCamera_orbital;
    m_zobCamera = c;
}

ZobCameraController::~ZobCameraController()
{
    m_zobCamera = NULL;
}

void ZobCameraController::Update()
{
	switch (m_type)
	{
	case Camera::eCamera_orbital:
		UpdateOrbital();
		break;
	default:
		break;
	}
}

void ZobCameraController::Rotate(float x, float y, float z)
{
    switch (m_type)
    {
    case Camera::eCamera_orbital:
        RotateOrbital(x, y, z);
        break;
    default:
        break;
    }
}

void ZobCameraController::Move(float x, float y, float z)
{
    switch (m_type)
    {
    case Camera::eCamera_orbital:
        MoveOrbital(x, y, z);
        break;
    default:
        break;
    }
}

void ZobCameraController::RotateOrbital(float x, float y, float z)
{
    ZobVector3 v = ZobVector3::Vector3Y;
    ZobVector3 p = ZobVector3::Vector3Zero;
    m_zobCamera->GetTargetVector(&p);
    m_zobCamera->RotateAroundPointAxis(&p, &v, NULL, x, false);
    v = m_zobCamera->GetLeft();
    m_zobCamera->RotateAroundPointAxis(&p, &v, NULL, y, false);
}

void ZobCameraController::MoveOrbital(float x, float y, float z)
{

}

void ZobCameraController::UpdateOrbital()
{
	ZobVector3 v = m_zobCamera->GetWorldPosition();
	if (m_zobCamera->m_tagetMode != Camera::eTarget_none)
	{
		if (m_zobCamera->m_tagetMode == Camera::eTarget_Vector && m_zobCamera->m_targetVector != v)
		{
			ZobVector3 p = m_zobCamera->GetWorldPosition();
			m_zobCamera->UpdateViewProjectionMatrix(&p, &m_zobCamera->m_targetVector, &ZobVector3::Vector3Y);
		}
		else if (m_zobCamera->m_tagetMode == Camera::eTarget_Object && m_zobCamera->m_targetObject)
		{
			//todo : cette section est a revoir
			const ZobVector3 p = m_zobCamera->GetWorldPosition();
			const ZobVector3 tp = m_zobCamera->m_targetObject->GetWorldPosition();
			ZobVector3 v = ZobVector3(tp);
			if (v != m_zobCamera->GetWorldPosition())
			{
				v = ZobVector3(tp);
				v = v - p;
				v.Normalize();
				//m_forward = v;
				//v = ZobVector3::GetAnglesFromVector(v);
				v = ZobMatrix4x4::QuaternionToEuler(v.x, v.z, v.z, 45.0f);
				m_zobCamera->SetWorldRotation(v.x, v.y, v.z);
				m_zobCamera->RecomputeFLUVectors(&v, &ZobVector3::Vector3Y);
			}
			m_zobCamera->UpdateViewProjectionMatrix(&p, &m_zobCamera->m_targetVector, &ZobVector3::Vector3Y);
		}
		else if (m_zobCamera->m_tagetMode == Camera::eTarget_FPS)
		{
			m_zobCamera->UpdateViewProjectionMatrix(&v, 0.0f, 0.0f);
		}
		//this will update the pysicnode orientation according to the lookAt
		m_zobCamera->LookAt(&m_zobCamera->m_forward, &m_zobCamera->m_left, &m_zobCamera->m_up, false);
	}
	else
	{
		m_zobCamera->UpdateViewProjectionMatrix(&v);
	}
}