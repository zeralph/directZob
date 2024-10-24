#include "ZobCameraController.h"
#include "../Rendering/Engine.h"

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

ZobCameraController::ZobCameraController(Camera* c, zobId guid) :ZobEntity(guid)
{
    SET_CLASS_AND_NAME
    m_cameraControllerType = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_cameraControllerType];
    m_zobObject = c;
    //c->m_varExposer->WrapVariable<std::string>("pouet Type", &m_typeName, NULL, true, false);
}

ZobCameraController::ZobCameraController(Camera * c, bool bEditorZobComponent) :ZobEntity((bEditorZobComponent ? ZobEntity::type_editor : ZobEntity::type_internal), ZobEntity::subtype_cameraController)
{
    SET_CLASS_AND_NAME
	m_cameraControllerType = Camera::eCamera_base;
    m_typeName = eCameraTypeStr[m_cameraControllerType];
    m_zobObject = c;
    //c->m_varExposer->WrapVariable<std::string>("pouet 2 Type", &m_typeName, NULL, true, false);
}

ZobCameraController::~ZobCameraController()
{
    m_zobObject = NULL;
}

void ZobCameraController::Update(float dt, bool isPlaying)
{
    //ZobVector3 v = m_zobObject->GetWorldPosition();
    //m_zobObject->UpdateViewProjectionMatrix(&v);
}

void ZobCameraController::Init(DirectZobType::sceneLoadingCallback cb)
{
    if (m_type != ZobEntity::type_editor && cb)
    {
        cb(0, 0, m_name);
    }
}

void ZobCameraController::SetTarget(const ZobVector3* t)
{

}

void ZobCameraController::PreUpdate(float dt, bool isPlaying)
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

ZobCameraController* ZobCameraController::Duplicate()
{
    assert(false);
    return NULL;
}