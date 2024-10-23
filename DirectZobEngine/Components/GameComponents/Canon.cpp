#include "Canon.h"
#include "../../ZobObjects/ZobObject.h"
#include "../../Misc/ZobVariablesExposer.h"

Canon::Canon(ZobObject* zobObject): ZobComponent(zobObject, false)
{
    SET_CLASS_AND_NAME
    m_componentType = ZobComponentFactory::eComponent_canon;
    Init(NULL);
    m_varExposer->WrapVariable<float>("Speed", &m_fireSpeed, NULL, false, true);
}
Canon::~Canon()
{

}
void Canon::Init(DirectZobType::sceneLoadingCallback cb)
{
    
}
void Canon::PreUpdate(float dt)
{

}
void Canon::PostUpdate()
{

}
void Canon::EditorUpdate()
{

}
void Canon::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const 
{

}