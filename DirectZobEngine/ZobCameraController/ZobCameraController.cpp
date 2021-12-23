#include "ZobCameraController.h"

const char* ZobCameraController::eCameraTypeStr[Camera::eCameraType::__eCameraType_MAX__] =
{
		"Base",
		"Revolving",
		"Orbital",
		"Fps",
		"Follow",
		"FollowCar"
};

Camera::eCameraType ZobCameraController::TypeFromString(const char* typeStr)
{
    if (typeStr)
    {
        for (int i = 0; i < Camera::eCameraType::__eCameraType_MAX__; i++)
        {
            if (strcmp(ZobCameraController::eCameraTypeStr[i], typeStr) == 0)
            {
                return (Camera::eCameraType)i;
            }
        }
    }
    return Camera::eCameraType::eCamera_base;
}

ZobCameraController::ZobCameraController(Camera* c, zobId guid) :ZOBGUID(guid)
{
    m_cameraControllerType = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_cameraControllerType];
    m_zobCamera = c;
    //c->m_varExposer->WrapVariable<std::string>("pouet Type", &m_typeName, NULL, true, false);
}

ZobCameraController::ZobCameraController(Camera * c) :ZOBGUID(ZOBGUID::type_internal, ZOBGUID::subtype_cameraController)
{
	m_cameraControllerType = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_cameraControllerType];
    m_zobCamera = c;
    //c->m_varExposer->WrapVariable<std::string>("pouet 2 Type", &m_typeName, NULL, true, false);
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

void ZobCameraController::SetTarget(const ZobVector3* t)
{

}

void ZobCameraController::PreUpdate(float dt)
{
}

void ZobCameraController::Rotate(float x, float y, float z)
{
}

void ZobCameraController::Move(float x, float y, float z)
{
}

void ZobCameraController::Zoom(float f)
{

}

void ZobCameraController::DrawGizmos(const Camera* camera, Engine* engine) const
{
    if (!engine->DrawCameraGizmos())
    {
        return;
    }
}

