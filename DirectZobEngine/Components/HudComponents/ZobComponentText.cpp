#include "ZobComponentText.h"
#include "../../DirectZob.h"
#include "../../Rendering/Engine.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"
#include "../../ZobPhysic/ZobPhysicComponent.h"
#include "../../Managers/ZobInputManager.h"
#include "../../Managers/ZobHUDManager.h"
#include "../../Rendering/text2D.h"
#include "../../ZobObjects/ZobObject.h"
#include "../../SceneLoader.h"
#include "../../Managers/ZobHUDManager.h"

ZobComponentText::~ZobComponentText()
{

}

ZobComponentText::ZobComponentText(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	SET_CLASS_AND_NAME
	m_x = 0;
	m_y = 0;
	m_w = 0.5f;
	m_h = 0.5f;
	m_z = 0;
	m_fontSize = 10;
	m_fontName = "Arial";
	m_text ="";
	m_action = eMenuAction::eAction_none;
	m_font = NULL;
	m_selectable = false;
	m_visible = true;
	m_defaultColor = ZobColor::White;
	m_selectedColor = ZobColor::Red;
	m_type = eComponent_hudText; 
	//m_unit = ZobHUDManager::eHudUnit_ratio;
	m_fontXml.SetFileType(ZobFilePath::eFileType_xml);
	m_fontImage.SetFileType(ZobFilePath::eFileType_texture);
	m_actionArg = "";
	m_selected = false;
	//m_varExposer->WrapAction("Add", &ZobComponentText::AddItem);
	m_varExposer->WrapVariable<bool>("Visible", &m_visible, NULL, false, true);
	m_varExposer->WrapVariable<float>("X", &m_x, NULL, false, true);
	m_varExposer->WrapVariable<float>("Y", &m_y, NULL, false, true);
	m_varExposer->WrapVariable<float>("W", &m_w, NULL, false, true);
	m_varExposer->WrapVariable<float>("H", &m_h, NULL, false, true);
	m_varExposer->WrapVariable<float>("Z", &m_z, NULL, false, true);
	m_varExposer->StartGroup("Font");
	m_varExposer->WrapVariable<float>("Font size", &m_fontSize, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Font Xml", &m_fontXml, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Font Image", &m_fontImage, NULL, false, true);
	m_varExposer->WrapVariable<std::string>("Text", &m_text, NULL, false, true);
	m_varExposer->EndGroup();
	m_varExposer->StartGroup("Selection");
	m_varExposer->WrapVariable<ZobColor>("Default color", &m_defaultColor, NULL, false, true);
	m_varExposer->WrapVariable<ZobColor>("Selected color", &m_selectedColor, NULL, false, true);
	m_varExposer->WrapVariable<bool>("Selectable", &m_selectable, &ZobComponentText::UpdateSelectableStatus, false, true);
	eMenuAction ma[3] = { eMenuAction::eAction_none,eMenuAction::eAction_Load, eMenuAction::eAction_exit };
	const char* maStr[3] = { "None", "Load", "Exit" };
	m_varExposer->WrapEnum<eMenuAction>("Action", &m_action, 3, ma, maStr, NULL, false, true);
	m_varExposer->WrapVariable<std::string>("Action argument", &m_actionArg, NULL, false, true);
	m_varExposer->EndGroup();
	m_varExposer->Load();
	Init();
}

void ZobComponentText::PreUpdate(float dt)
{
	if (m_visible)
	{
		ZobHUDManager* hud = DirectZob::GetInstance()->GetHudManager();
		if (m_font && m_text.length() > 0)
		{
			if (m_selected)
			{
				hud->Print(ZobHUDManager::eHudUnit_ratio, m_x, m_y, m_z, m_fontSize, m_font, &m_selectedColor, m_text.c_str());
			}
			else
			{
				hud->Print(ZobHUDManager::eHudUnit_ratio, m_x, m_y, m_z, m_fontSize, m_font, &m_defaultColor, m_text.c_str());
			}
		}
	}
}

void ZobComponentText::EditorUpdate()
{
	if (!m_font)
	{
		if (m_fontImage.IsDefined() && m_fontXml.IsDefined())
		{
			LoadFontInternal();
		}
	}
}

void ZobComponentText::LoadFontInternal()
{
	if (m_fontXml.IsDefined() && m_fontImage.IsDefined())
	{
		if (!m_font || ZobFont::GetFontNameFromXml(m_fontXml) != m_font->GetName())
		{
			m_font = DirectZob::GetInstance()->GetHudManager()->CreateOrGetZobFont(m_fontXml, m_fontImage);
		}
	}
	else if (m_font)
	{
		m_font = NULL;
	}
}

void ZobComponentText::Init()
{
	m_varExposer->Load();
	LoadFontInternal();
	if (m_selectable)
	{
		DirectZob::GetInstance()->GetHudManager()->AddToMenu(this);
	}
}

void ZobComponentText::UpdateSelectableStatus(zobId id)
{
	ZobComponentText* z = ZobEntity::GetEntity<ZobComponentText>(id);
	if (z)
	{
		if (z->m_selectable)
		{
			DirectZob::GetInstance()->GetHudManager()->AddToMenu(z);
		}
		else
		{
			DirectZob::GetInstance()->GetHudManager()->RemoveFromMenu(z);
		}
	}
}

void ZobComponentText::SetSelected(bool b)
{
	m_selected = b;
}

void ZobComponentText::DoAction()
{
	if (m_action == eMenuAction::eAction_Load && m_actionArg.length() > 0)
	{
		std::string s = SceneLoader::GetResourcePath();
		SceneLoader::LoadScene(s, m_actionArg);
	}
	else if (m_action == eMenuAction::eAction_exit)
	{
		DirectZob::GetInstance()->Exit();
	}
}

void ZobComponentText::UpdateBeforeObject(float dt)
{

}

void ZobComponentText::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}
