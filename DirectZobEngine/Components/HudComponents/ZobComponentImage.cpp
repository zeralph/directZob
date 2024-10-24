#include "ZobComponentImage.h"
#include "../../DirectZob.h"
#include "../../Rendering/Engine.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"
#include "../../ZobPhysic/ZobPhysicComponent.h"
#include "../../Managers/ZobInputManager.h"
#include "../../Managers/ZobHUDManager.h"
#include "../../Managers/MaterialManager.h"
#include "../../Rendering/text2D.h"
#include "../../ZobObjects/ZobObject.h"

ZobComponentImage::~ZobComponentImage()
{

}

ZobComponentImage::ZobComponentImage(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	SET_CLASS_AND_NAME
	m_x = 0;
	m_y = 0;
	m_w = 0.5f;
	m_h = 0.5f;
	m_z = 0;
	m_visible = true;
	m_componentType = ZobComponentFactory::eComponent_hudImage;
	//m_varExposer->WrapAction("Add", &ZobComponentImage::AddItem);
	m_texture.SetFileType(ZobFilePath::eFileType_image);
	m_varExposer->WrapVariable<bool>("Visible", &m_visible, NULL, false, true);
	m_varExposer->WrapVariable<float>("X", &m_x, NULL, false, true);
	m_varExposer->WrapVariable<float>("Y", &m_y, NULL, false, true);
	m_varExposer->WrapVariable<float>("W", &m_w, NULL, false, true);
	m_varExposer->WrapVariable<float>("H", &m_h, NULL, false, true);
	m_varExposer->WrapVariable<float>("Z", &m_z, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Image", &m_texture, NULL, false, true);
	m_varExposer->Load(false);
	Init(NULL);
}

void ZobComponentImage::PreUpdate(float dt, bool isPlaying)
{
	if (m_visible)
	{
		ZobHUDManager* hud = DirectZob::GetInstance()->GetHudManager();
		if (m_image)
		{
			hud->Print(m_zobObject, ZobHUDManager::eHudUnit_ratio, m_x, m_y, m_w, m_h, m_z, m_image);
		}
	}
}

void ZobComponentImage::EditorUpdate(bool isPlaying)
{
	if (!m_image)
	{
		if (m_texture.IsDefined())
		{
			LoadMaterialInternal();
		}
	}
	else
	{
		if (m_texture.GetName() != m_image->GetName())
		{
			LoadMaterialInternal();
		}
	}
}

void ZobComponentImage::LoadMaterialInternal()
{
	if (m_texture.IsDefined())
	{
		if (!m_image || m_texture.GetName() != m_image->GetName())
		{
			m_image = DirectZob::GetInstance()->GetMaterialManager()->GetMaterial(m_texture.GetName());
			if (!m_image)
			{
				ZobColor a = ZobColor::White;
				m_image = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texture.GetName(), &a, &a, &a, 0, &m_texture);
			}
		}
	}
	else if (m_image)
	{
		m_image = NULL;
	}
}


void ZobComponentImage::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (m_type != ZobEntity::type_editor && cb)
	{
		cb(0, 0, m_name);
	}
	m_varExposer->Load(true);
	LoadMaterialInternal();
}

void ZobComponentImage::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}
