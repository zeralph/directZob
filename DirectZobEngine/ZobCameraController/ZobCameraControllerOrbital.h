#pragma once
#include "ZobCameraController.h"
#include "ZobObjects/Camera.h"
class ZobCameraControllerOrbital : public ZobCameraController
{
public:
    ZobCameraControllerOrbital(Camera * c);
    ~ZobCameraControllerOrbital();

    void        Update() override;
    void        PreUpdate() override;
    void        Rotate(float x, float y, float z) override;
    void        Move(float x, float y, float z) override;
private:

    float m_rotationX;
    float m_rotationY;
    float m_moveX;
    float m_moveY; 
    float m_moveZ;
};