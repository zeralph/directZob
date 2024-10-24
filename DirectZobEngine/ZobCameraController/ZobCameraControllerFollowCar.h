#pragma once
#include "ZobCameraController.h"
#include "../ZobPhysic/ZobPhysicComponent.h"

class Camera;
class ZobCameraControllerFollowCar: public ZobCameraController
{
public:
    ZobCameraControllerFollowCar(Camera* c, zobId guid);
    ZobCameraControllerFollowCar(Camera * c, bool bEditorZobComponent);
    ~ZobCameraControllerFollowCar();

    void            Init(DirectZobType::sceneLoadingCallback cb) override;
    void            Update(float dt, bool isPlaying) override;
    void            PreUpdate(float dt, bool isPlaying) override;
    void            Rotate(float x, float y, float z) override;
    void            Move(float x, float y, float z) override;
    void            DrawGizmos(const Camera* camera, Engine* engine) const override;
    std::string             GetTypeName() const { return m_typeName; }
private:

protected:
    float m_cumulatedExtraRotation;
    float m_startExtraRotation;
    float m_rotationSpeed;
    bool m_firstInit;
    float m_deltaY;
    float m_accY;
    Transform m_startTransform;
    ZobVector3 m_distanceToCar;
    Quaternion m_lastOrientation;
};