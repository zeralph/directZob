#include "ZobCameraControllerFollowCar.h"
#include "../Managers/ZobInputManager.h"
#include "../DirectZob.h"

ZobCameraControllerFollowCar::ZobCameraControllerFollowCar(Camera * c) :ZobCameraController(c)
{
	m_type = Camera::eCamera_followCar;
    m_typeName = "CameraFollowCar";
    m_zobCamera = c;
    m_cumulatedExtraRotation = 0.0f;
    m_rotationSpeed = 1.0f;
    m_deltaY = 0.0f;
    m_accY = 0.0f;
    m_distanceToCar = ZobVector3(0, 0, 0);
}

ZobCameraControllerFollowCar::~ZobCameraControllerFollowCar()
{
    m_zobCamera = NULL;
}

void ZobCameraControllerFollowCar::Init()
{
    Vector3 p = m_zobCamera->m_parent->GetPhysicComponent()->GetWorldTransform().getPosition();
    Vector3 m = m_zobCamera->GetPhysicComponent()->GetWorldTransform().getPosition();
    m_distanceToCar.x = m.x - p.x;
    m_distanceToCar.y = m.y - p.y;
    m_distanceToCar.z = m.z - p.z;
    m_startTransform = m_zobCamera->GetPhysicComponent()->GetLocalTransform();
    m_lastOrientation = m_startTransform.getOrientation();
}

void ZobCameraControllerFollowCar::Update(float dt)
{
    Transform parentT = m_zobCamera->m_parent->GetPhysicComponent()->GetWorldTransform(); 
    Quaternion q = parentT.getOrientation();
    parentT.setOrientation(m_lastOrientation);
    Transform newT = m_startTransform;
    newT = parentT * newT;
    parentT = m_zobCamera->m_parent->GetPhysicComponent()->GetWorldTransform();
    newT = parentT.getInverse() * newT;
    m_zobCamera->GetPhysicComponentNoConst()->SetLocalTransform(newT);
    m_deltaY = dt;
    float d = 0.09f;
    q = Quaternion::slerp(m_lastOrientation, q, d);
    m_lastOrientation = q;
}

void ZobCameraControllerFollowCar::PreUpdate()
{
    if (m_zobCamera->m_active)
    {
        float parentY = m_zobCamera->m_parent->GetWorldRotation().y;
        float myY = m_zobCamera->GetWorldRotation().y;
        //m_deltaY = parentY - myY;
        const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
        float mx = fabsf(inputMap->GetFloat(ZobInputManager::LeftStickX));
        //m_deltaY += mx;
        //m_deltaY = fminf(0.5f, m_deltaY);
    }
}

void ZobCameraControllerFollowCar::Rotate(float x, float y, float z)
{
}

void ZobCameraControllerFollowCar::Move(float x, float y, float z)
{
}

void ZobCameraControllerFollowCar::DrawGizmos(const Camera* camera, Core::Engine* engine) const
{
    Text2D* m_textManager = DirectZob::GetInstance()->GetTextManager();
    m_textManager->Print(50, 200, 4, 0xFFFFFF, "ACC : %.2f, dy : %.2f", m_accY, m_deltaY);
}