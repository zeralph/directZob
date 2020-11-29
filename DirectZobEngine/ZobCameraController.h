#pragma once
#include "Camera.h"
class ZobCameraController
{
public:
    ZobCameraController(Camera * c);
    ~ZobCameraController();

    void SetType(Camera::eCameraType type) {m_type = type;};
    void Update();

    //void MoveHorizontal 

private:
    Camera* m_zobCamera;
    Camera::eCameraType m_type;
};