#pragma once
#include "ZobObjects/Camera.h"
class ZobCameraController 
{
public:
    ZobCameraController(Camera * c);
    ~ZobCameraController();

    virtual void            Update();
    virtual void            Rotate(float x, float y, float z);
    virtual void            Move(float x, float y, float z);
    Camera::eCameraType     GetType() const { return m_type; }
private:

protected:
    Camera* m_zobCamera;
    Camera::eCameraType m_type;
};