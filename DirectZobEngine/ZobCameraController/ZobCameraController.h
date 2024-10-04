#pragma once
#include "../ZobObjects/Camera.h"
#include "../ZobObjects/ZobEntity.h"
class ZobCameraController : public ZobEntity
{
public:
    ZobCameraController(Camera* c, bool bEditorZobComponent);
    ZobCameraController(Camera* c, zobId guid);
    ~ZobCameraController();


    static const char*          eCameraTypeStr[Camera::eCameraType::__eCameraType_MAX__];
    virtual void                Init(DirectZobType::sceneLoadingCallback cb) override;
    virtual void                Update(float dt);
    virtual void                PreUpdate(float dt);
    virtual void                Rotate(float x, float y, float z);
    virtual void                Move(float x, float y, float z);
    virtual void                Zoom(float f);
    virtual void                DrawGizmos(const Camera* camera, Engine* engine) const;
    virtual void                SetTarget(const ZobVector3* t);
    ZobCameraController*        Duplicate() override;
    Camera::eCameraType         GetType() const { return m_cameraControllerType; }
    static Camera::eCameraType  TypeFromString(const char* typeStr);
    const char*                 GetTypeName() const { return ZobCameraController::eCameraTypeStr[m_cameraControllerType]; }
private:

protected:
    Camera* m_zobObject;
    Camera::eCameraType m_cameraControllerType;
    std::string m_typeName;
};