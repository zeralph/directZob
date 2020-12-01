#pragma once
#include "ZobCameraController.h"
#include "ZobObjects/Camera.h"
class ZobCameraControllerOrbital : public ZobCameraController
{
public:
    ZobCameraControllerOrbital(Camera * c);
    ~ZobCameraControllerOrbital();

    void        Update() override;

    void        Rotate(float x, float y, float z) override;
    void        Move(float x, float y, float z) override;
private:

};