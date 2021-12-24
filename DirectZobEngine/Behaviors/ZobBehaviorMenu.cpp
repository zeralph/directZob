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
	m_type = eBehavior_menu; 
}

void ZobBehaviorMenu::PreUpdate(float dt)
{
	ZobHUDManager* hud = DirectZob::GetInstance()->GetHudManager();
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	m_time += dt * 2.0f;
	float f = 2.0f + (0.5f + sinf(m_time) / 2.0f) * 5.0f;
	hud->Print(ZobHUDManager::eHudUnit_ratio, 0.25f, 0.25f, f, "Leelawadee UI", &ZobColor::Red, "EDGE RACING");
	for (int i = 0; i < m_menuEntries.size(); i++)
	{
		float h = 0.5 + i * 0.05f;
		if (i == m_menuIndex)
		{
			hud->Print(ZobHUDManager::eHudUnit_ratio, 0.2f, h, 2, "Arial", &ZobColor::Red, "%s %s",  "->", m_menuEntries[i].name.c_str());
		}
		else
		{
			hud->Print(ZobHUDManager::eHudUnit_ratio, 0.2f, h, 2, "Arial", &ZobColor::Red, "%s %s", "   ", m_menuEntries[i].name.c_str());
		}
		std::string s = m_menuEntries[i].name;

	}

	if (inputMap->GetBoolIsNew(ZobInputManager::MenuDown))
	{
		m_menuIndex++;
		if (m_menuIndex >= m_menuEntries.size())
		{
			m_menuIndex = m_menuEntries.size() - 1;
		}
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::MenuUp))
	{
		m_menuIndex--;
		if (m_menuIndex <= 0)
		{
			m_menuIndex = 0;
		}
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::Start))
	{
		MenuEntry m = m_menuEntries[m_menuIndex];
		switch (m.action)
		{
		case eAction_Load:
		{
			std::string sceneName = m.file;
			std::string scenePath = DirectZob::GetInstance()->GetResourcePath();
			DirectZob::GetInstance()->LoadScene(scenePath, sceneName, NULL);
			break;
		}
		case eAction_exit:
		{
			DirectZob::GetInstance()->Exit();
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void ZobBehaviorMenu::Init()
{
	m_time = 0;
	m_menuIndex = 0;
	MenuEntry m0;
	m0.file = "carTest.dzs";
	m0.name = "Car Test";
	m0.action = eAction_Load;
	m_menuEntries.push_back(m0);

	MenuEntry m1;
	m1.file = "testGround.dzs";
	m1.name = "Test Ground";
	m1.action = eAction_Load;
	m_menuEntries.push_back(m1);

	MenuEntry m2;
	m2.file = "";
	m2.name = "Exit";
	m2.action = eAction_exit;
	m_menuEntries.push_back(m2);
}

void ZobBehaviorMenu::EditorUpdate()
{

}

void ZobBehaviorMenu::UpdateBeforeObject(float dt)
{

}

void ZobBehaviorMenu::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}