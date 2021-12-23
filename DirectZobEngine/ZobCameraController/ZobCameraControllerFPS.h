#pragma once
#include "ZobCameraController.h"
#include "ZobObjects/Camera.h"
class ZobCameraControllerFPS : public ZobCameraController
{
public:
    ZobCameraControllerFPS(Camera* c);
    ZobCameraControllerFPS(Camera * c, zobId guid);
    ~ZobCameraControllerFPS();

    void        Update(float dt) override;
    void        PreUpdate(float dt) override;
    void        Rotate(float x, float y, float z) override;
    void        Move(float x, float y, float z) override;
private:
        
    float m_deltaPitch;
    float m_deltaYaw;
    float m_deltaRoll;
    float m_totalPitch;
    float m_totalYaw;
    float m_totalRoll;
    float m_moveX;
    float m_moveY;
    float m_moveZ;
};