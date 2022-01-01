#include "ZobBehaviorMenu.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Managers/ZobHUDManager.h"
#include "../Rendering/text2D.h"
#include "../ZobObjects/ZobObject.h"

ZobBehaviorMenu::~ZobBehaviorMenu()
{

}

ZobBehaviorMenu::ZobBehaviorMenu(ZobObject* zobObject, bool bEditorZobBehavior) : ZobBehavior(zobObject, bEditorZobBehavior)
{
	m_x = 0;
	m_y = 0;
	m_w = 0.5f;
	m_h = 0.5f;
	m_fontSize = 10;
	m_fontName = "Arial";
	m_text ="";
	m_image = NULL;
	m_type = eBehavior_hudElement; 
	m_unit = ZobHUDManager::eHudUnit_ratio;
	//m_varExposer->WrapAction("Add", &ZobBehaviorMenu::AddItem);
	m_varExposer->WrapVariable<float>("X", &m_x, NULL, false, true);
	m_varExposer->WrapVariable<float>("Y", &m_y, NULL, false, true);
	m_varExposer->WrapVariable<float>("W", &m_w, NULL, false, true);
	m_varExposer->WrapVariable<float>("H", &m_h, NULL, false, true);
	m_varExposer->WrapVariable<float>("Font size", &m_fontSize, NULL, false, true);
	m_varExposer->WrapVariable<std::string>("Text", &m_text, NULL, false, true);
	m_varExposer->WrapVariable<ZobColor>("Color", &m_color, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Image", &m_texture, NULL, false, true);
	m_varExposer->Load();
	Init();
}

void ZobBehaviorMenu::PreUpdate(float dt)
{
	ZobHUDManager* hud = DirectZob::GetInstance()->GetHudManager();
	if (m_text.length() > 0)
	{
		hud->Print(m_unit, m_x, m_y, m_fontSize, m_fontName, &m_color, m_text.c_str()); 
	}
	if (m_image)
	{
		hud->Print(m_unit, m_x, m_y, m_w, m_h, m_image);
	}
}

void ZobBehaviorMenu::EditorUpdate()
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

void ZobBehaviorMenu::LoadMaterialInternal()
{
	if (m_texture.IsDefined())
	{
		if (!m_image || m_texture.GetName() != m_image->GetName())
		{
			m_image = DirectZob::GetInstance()->GetMaterialManager()->GetMaterial(m_texture.GetName());
			if (!m_image)
			{
				ZobColor a = ZobColor::White;
				m_image = DirectZob::GetInstance()->GetMaterialManager()->LoadMaterial(m_texture.GetName(), &a, &a, &a, 0, false, m_texture);
			}
		}
	}
	else if (m_image)
	{
		m_image = NULL;
	}

}

void ZobBehaviorMenu::Init()
{
	m_varExposer->Load();
	LoadMaterialInternal();
}

void ZobBehaviorMenu::UpdateBeforeObject(float dt)
{

}

void ZobBehaviorMenu::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}
