#include "Canon.h"
#include "../../ZobObjects/ZobObject.h"
#include "../../Misc/ZobVariablesExposer.h"
#include "../../Managers/ZobObjectManager.h"
#include "../../SceneLoader.h"
#include "../../DirectZob.h"
Canon::Canon(ZobObject* zobObject): ZobComponent(zobObject, false)
{
    SET_CLASS_AND_NAME
    m_componentType = ZobComponentFactory::eComponent_canon;
    Init(NULL);
    m_varExposer->WrapVariable<float>("Speed", &m_fireSpeed, NULL, false, true);
    m_bulletAsset.SetFileType(ZobFilePath::eFileType_asset);
    m_varExposer->WrapVariable<ZobFilePath>("File", &m_bulletAsset, NULL, false, true);
}
Canon::~Canon()
{

}
void Canon::Init(DirectZobType::sceneLoadingCallback cb)
{
    
}
void Canon::Start()
{
    m_timer = 0;
}
void Canon::PreUpdate(float dt, bool isPlaying)
{
    if (isPlaying)
    {
        m_timer += dt;
        if (m_timer >= m_fireSpeed)
        {
            m_timer = 0;
            
            ZobObject* root = DirectZob::GetInstance()->GetZobObjectManager()->GetRootObject();
            ZobObject* bullet = SceneLoader::LoadAsset(m_zobObject, m_bulletAsset.GetPath(), m_bulletAsset.GetFile());
            bullet->SetParent(root);
        }
    }
}
void Canon::PostUpdate(bool isPlaying)
{

}
void Canon::EditorUpdate(bool isPlaying)
{

}
void Canon::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const 
{

}