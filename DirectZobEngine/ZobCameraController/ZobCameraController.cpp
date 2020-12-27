#include "ZobCameraController.h"


ZobCameraController::ZobCameraController(Camera * c)
{
	m_type = Camera::eCamera_base;
    m_typeName = "baseCamera";
    m_zobCamera = c;
}

ZobCameraController::~ZobCameraController()
{
    m_zobCamera = NULL;
}

void ZobCameraController::Update(float dt)
{
    //ZobVector3 v = m_zobCamera->GetWorldPosition();
    //m_zobCamera->UpdateViewProjectionMatrix(&v);
}

void ZobCameraController::Init()
{

}

void ZobCameraController::PreUpdate()
{
}

void ZobCameraController::Rotate(float x, float y, float z)
{
}

void ZobCameraController::Move(float x, float y, float z)
{
}

void ZobCameraController::DrawGizmos(const Camera* camera, Core::Engine* engine) const
{

}

