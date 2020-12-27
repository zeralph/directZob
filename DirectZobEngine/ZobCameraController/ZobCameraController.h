#pragma once
#include "ZobObjects/Camera.h"
class ZobCameraController 
{
public:
    ZobCameraController(Camera * c);
    ~ZobCameraController();

    virtual void            Init();
    virtual void            Update(float dt);
    virtual void            PreUpdate();
    virtual void            Rotate(float x, float y, float z);
    virtual void            Move(float x, float y, float z);
    virtual void            DrawGizmos(const Camera* camera, Core::Engine* engine) const;
    Camera::eCameraType     GetType() const { return m_type; }
    std::string             GetTypeName() const { return m_typeName; }
private:

protected:
    Camera* m_zobCamera;
    Camera::eCameraType m_type;
    std::string m_typeName;
};