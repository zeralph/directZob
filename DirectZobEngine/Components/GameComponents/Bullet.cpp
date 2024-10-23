#include "Bullet.h"
#include "../../ZobObjects/ZobObject.h"
#include "../../Misc/ZobVariablesExposer.h"
Bullet::Bullet(ZobObject* zobObject): ZobComponent(zobObject, false)
{
    SET_CLASS_AND_NAME
    m_componentType = ZobComponentFactory::eComponent_bullet;
    Init(NULL);
    m_varExposer->WrapVariable<float>("LifeTime", &m_lifeTimeSec, NULL, false, true);
    m_varExposer->WrapVariable<float>("Speed", &m_speed, NULL, false, true);
}
Bullet::~Bullet()
{

}
void Bullet::Init(DirectZobType::sceneLoadingCallback cb)
{
    ReLoadVariables();
}
void Bullet::PreUpdate(float dt)
{

}
void Bullet::PostUpdate()
{

}
void Bullet::EditorUpdate()
{

}
void Bullet::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const 
{

}