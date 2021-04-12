#pragma once
#include "ZobObjects/Camera.h"
#include "ZobObjects/ZOBGUID.h"
class ZobCameraController : public ZOBGUID
{
public:
    ZobCameraController(Camera* c);
    ZobCameraController(Camera* c, std::string guid);
    ~ZobCameraController();


    static const char*          eCameraTypeStr[Camera::eCameraType::__eCameraType_MAX__];
    virtual void                Init();
    virtual void                Update(float dt);
    virtual void                PreUpdate();
    virtual void                Rotate(float x, float y, float z);
    virtual void                Move(float x, float y, float z);
    virtual void                Zoom(float f);
    virtual void                DrawGizmos(const Camera* camera, Core::Engine* engine) const;
    Camera::eCameraType         GetType() const { return m_type; }
    static Camera::eCameraType  TypeFromString(const char* typeStr);
    const char*                 GetTypeName() const { return ZobCameraController::eCameraTypeStr[m_type]; }
private:

protected:
    Camera* m_zobCamera;
    Camera::eCameraType m_type;
    std::string m_typeName;
};