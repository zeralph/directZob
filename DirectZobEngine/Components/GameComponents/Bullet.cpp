#include "Bullet.h"
#include "../../ZobObjects/ZobObject.h"
#include "../../DirectZob.h"
#include "../../Managers/ZobObjectManager.h"
#include "../../Misc/ZobVariablesExposer.h"
Bullet::Bullet(ZobObject* zobObject): ZobComponent(zobObject, false)
{
    SET_CLASS_AND_NAME
    m_componentType = ZobComponentFactory::eComponent_bullet;
    Init(NULL);
    m_varExposer->WrapVariable<float>("LifeTime", &m_lifeTimeSec, NULL, false, true);
    m_varExposer->WrapVariable<float>("Speed", &m_speed, NULL, false, true);
    m_varExposer->WrapVariable<float>("Remaining", &m_remainingTime, NULL, true, false);  
}
Bullet::~Bullet()
{

}
void Bullet::Init(DirectZobType::sceneLoadingCallback cb)
{
    ReLoadVariables();
}
void Bullet::Start()
{
    m_remainingTime = m_lifeTimeSec;
}
void Bullet::PreUpdate(float dt, bool isPlaying)
{
    if (isPlaying)
    {
        ZobObject* z = GetZobObject();
        ZobVector3 v = z->GetForward();
        v.Mul(dt * m_speed);
        v.Add(&z->GetWorldPosition());
        z->SetWorldPosition(v.x, v.y, v.z);
        m_remainingTime -= dt;
        if (m_remainingTime < 0)
        {
            DirectZob::GetInstance()->GetZobObjectManager()->RemoveZobObject(m_zobObject);
        }
    }
}
void Bullet::PostUpdate(bool isPlaying)
{

}
void Bullet::EditorUpdate(bool isPlaying)
{

}
void Bullet::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const 
{

}