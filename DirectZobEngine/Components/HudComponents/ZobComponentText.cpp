#include "ZobComponentText.h"
#include "../../DirectZob.h"
#include "../../Rendering/Engine.h"
#include "../../ZobPhysic/ZobPhysicsEngine.h"
#include "../../ZobPhysic/ZobPhysicComponent.h"
#include "../../Managers/ZobInputManager.h"
#include "../../Managers/ZobHUDManager.h"
#include "../../Rendering/text2D.h"
#include "../../ZobObjects/ZobObject.h"

ZobComponentText::~ZobComponentText()
{

}

ZobComponentText::ZobComponentText(ZobObject* zobObject, bool bEditorZobComponent) : ZobComponent(zobObject, bEditorZobComponent)
{
	m_x = 0;
	m_y = 0;
	m_w = 0.5f;
	m_h = 0.5f;
	m_z = 0;
	m_fontSize = 10;
	m_fontName = "Arial";
	m_text ="";
	m_font = NULL;
	m_visible = true;
	m_type = eComponent_hudText; 
	m_unit = ZobHUDManager::eHudUnit_ratio;
	m_fontXml.SetFileType(ZobFilePath::eFileType_xml);
	m_fontImage.SetFileType(ZobFilePath::eFileType_texture);
	//m_varExposer->WrapAction("Add", &ZobComponentText::AddItem);
	
	m_varExposer->WrapVariable<bool>("Visible", &m_visible, NULL, false, true);
	m_varExposer->WrapVariable<float>("X", &m_x, NULL, false, true);
	m_varExposer->WrapVariable<float>("Y", &m_y, NULL, false, true);
	m_varExposer->WrapVariable<float>("W", &m_w, NULL, false, true);
	m_varExposer->WrapVariable<float>("H", &m_h, NULL, false, true);
	m_varExposer->WrapVariable<float>("Z", &m_z, NULL, false, true);
	m_varExposer->WrapVariable<float>("Font size", &m_fontSize, NULL, false, true);
	m_varExposer->WrapVariable<std::string>("Text", &m_text, NULL, false, true);
	m_varExposer->WrapVariable<ZobColor>("Color", &m_color, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Font Xml", &m_fontXml, NULL, false, true);
	m_varExposer->WrapVariable<ZobFilePath>("Font Image", &m_fontImage, NULL, false, true);
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
			hud->Print(m_unit, m_x, m_y, m_z, m_fontSize, m_font, &m_color, m_text.c_str());
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
}

void ZobComponentText::UpdateBeforeObject(float dt)
{

}

void ZobComponentText::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}
