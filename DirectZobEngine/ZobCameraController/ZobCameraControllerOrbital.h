#pragma once
#include "ZobCameraController.h"
#include "ZobObjects/Camera.h"
#include "string.h"

class ZobCameraControllerOrbital : public ZobCameraController
{
public:
    ZobCameraControllerOrbital(Camera* c, bool isFree, zobId guid);
    ZobCameraControllerOrbital(Camera * c, bool isFree);
    ~ZobCameraControllerOrbital();

    void        Update(float dt) override;
    void        PreUpdate(float dt) override;
    void        Rotate(float x, float y, float z) override;
    void        Move(float x, float y, float z) override;
    void        SetTarget(const ZobVector3* t) override;
    void        Zoom(float f) override;
private:

    ZobVector3  m_centerOfRotation;
    float       m_rotationX;
    float       m_rotationY;
    float       m_moveX;
    float       m_moveY; 
    float       m_moveZ;
    float       m_dist;
    bool        m_isFree;
};