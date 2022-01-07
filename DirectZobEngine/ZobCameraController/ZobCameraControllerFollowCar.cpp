#include "ZobCameraControllerFollowCar.h"
#include "../Managers/ZobInputManager.h"
#include "../DirectZob.h"

ZobCameraControllerFollowCar::ZobCameraControllerFollowCar(Camera* c, zobId guid) :ZobCameraController(c, guid)
{
    SET_CLASS_AND_NAME
    m_cameraControllerType = Camera::eCamera_followCar;
    m_zobObject = c;
    m_cumulatedExtraRotation = 0.0f;
    m_rotationSpeed = 1.0f;
    m_deltaY = 0.0f;
    m_accY = 0.0f;
    m_distanceToCar = ZobVector3(0, 0, 0);
}

ZobCameraControllerFollowCar::ZobCameraControllerFollowCar(Camera * c, bool bEditorZobComponent) :ZobCameraController(c, bEditorZobComponent)
{
    SET_CLASS_AND_NAME
	m_cameraControllerType = Camera::eCamera_followCar;
    m_zobObject = c;
    m_cumulatedExtraRotation = 0.0f;
    m_rotationSpeed = 1.0f;
    m_deltaY = 0.0f;
    m_accY = 0.0f;
    m_distanceToCar = ZobVector3(0, 0, 0);
}

ZobCameraControllerFollowCar::~ZobCameraControllerFollowCar()
{
    m_zobObject = NULL;
}

void ZobCameraControllerFollowCar::Init(DirectZobType::sceneLoadingCallback cb)
{
    ZobCameraController::Init(cb);
    Vector3 p = m_zobObject->m_parent->GetPhysicComponent()->GetWorldTransform().getPosition();
    Vector3 m = m_zobObject->GetPhysicComponent()->GetWorldTransform().getPosition();
    m_distanceToCar.x = m.x - p.x;
    m_distanceToCar.y = m.y - p.y;
    m_distanceToCar.z = m.z - p.z;
    m_startTransform = m_zobObject->GetPhysicComponent()->GetLocalTransform();
    m_lastOrientation = m_startTransform.getOrientation();
}

void ZobCameraControllerFollowCar::Update(float dt)
{
    Transform parentT = m_zobObject->m_parent->GetPhysicComponent()->GetWorldTransform(); 
    Quaternion q = parentT.getOrientation();
    parentT.setOrientation(m_lastOrientation);
    Transform newT = m_startTransform;
    newT = parentT * newT;
    parentT = m_zobObject->m_parent->GetPhysicComponent()->GetWorldTransform();
    //newT = parentT.getInverse() * newT;
    //m_zobObject->GetPhysicComponentNoConst()->SetLocalTransform(newT);
    m_zobObject->GetPhysicComponentNoConst()->SetWorldTransform(newT);
    m_deltaY = dt;
    float d = clamp((float)(dt * 10.0f), 0.0f, 1.0f);// 0.1f;
    //d = 1.0f;
    q = Quaternion::slerp(m_lastOrientation, q, d);
    m_lastOrientation = q;
}

void ZobCameraControllerFollowCar::PreUpdate(float dt)
{
    if (m_zobObject->m_active)
    {
    }
}

void ZobCameraControllerFollowCar::Rotate(float x, float y, float z)
{
}

void ZobCameraControllerFollowCar::Move(float x, float y, float z)
{
}

void ZobCameraControllerFollowCar::DrawGizmos(const Camera* camera, Engine* engine) const
{
    //Text2D* m_textManager = DirectZob::GetInstance()->GetTextManager();
    //m_textManager->Print(50, 200, 4, 0xFFFFFF, "ACC : %.2f, dy : %.2f", m_accY, m_deltaY);
}