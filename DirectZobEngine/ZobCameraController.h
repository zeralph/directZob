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
    void        Rotate(float x, float y, float z);
    void        Move(float x, float y, float z);
private:

    void RotateOrbital(float x, float y, float z);
    void MoveOrbital(float x, float y, float z);
    void UpdateOrbital();

    Camera* m_zobCamera;
    Camera::eCameraType m_type;
};