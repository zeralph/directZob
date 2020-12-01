#pragma once
#include "ZobCameraController.h"
#include "ZobObjects/Camera.h"
class ZobCameraControllerFPS : public ZobCameraController
{
public:
    ZobCameraControllerFPS(Camera * c);
    ~ZobCameraControllerFPS();

    void        Update() override;

    void        Rotate(float x, float y, float z) override;
    void        Move(float x, float y, float z) override;
private:
        
    float m_pitch;
    float m_yaw;
};