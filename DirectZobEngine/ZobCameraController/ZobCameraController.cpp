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

ZobCameraController::ZobCameraController(Camera* c, std::string guid) :ZOBGUID(guid)
{
    m_type = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_type];
    m_zobCamera = c;
}

ZobCameraController::ZobCameraController(Camera * c) :ZOBGUID(ZOBGUID::type_internal, ZOBGUID::subtype_cameraController)
{
	m_type = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_type];
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

void ZobCameraController::Zoom(float f)
{

}

void ZobCameraController::DrawGizmos(const Camera* camera, Core::Engine* engine) const
{
    if (!engine->DrawCameraGizmos())
    {
        return;
    }
}

