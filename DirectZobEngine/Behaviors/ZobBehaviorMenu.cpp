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

ZobBehaviorMenu::ZobBehaviorMenu(ZobObject* zobObject, TiXmlElement* node) : ZobBehavior(zobObject, node)
{
}

void ZobBehaviorMenu::PreUpdate()
{

}

void ZobBehaviorMenu::Init()
{
	m_time = 0;
}

void ZobBehaviorMenu::Update(float dt)
{
	ZobVector3 color = ZobVector3(255, 0, 0);
	ZobHUDManager* hud = DirectZob::GetInstance()->GetHudManager();
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	m_time += dt*2.0f;
	float f = 2.0f + (0.5f + sinf(m_time)/2.0f) * 5.0f;
	hud->Print(ZobHUDManager::eHudUnit_ratio, 0.25f, 0.25f, f, &color, "EDGE RACING");
	color = ZobVector3(1, 1, 0);
	hud->Print(ZobHUDManager::eHudUnit_ratio, 0.2f, 0.5f, 2, &color, "Press Start or Enter to load CarTest");
	hud->Print(ZobHUDManager::eHudUnit_ratio, 0.2f, 0.6f, 2, &color, "Press Select or Shift to load TesGround");
	hud->Print(ZobHUDManager::eHudUnit_ratio, 0.2f, 0.7f, 2, &color, "Press Start in game to go back to menu");
	if (inputMap->GetBoolIsNew(ZobInputManager::Start))
	{
		std::string sceneName = "carTest.dzs";
		std::string scenePath = DirectZob::GetInstance()->GetResourcePath();
		hud->Print(ZobHUDManager::eHudUnit_ratio, 0.25f, 0.5f, 4, &color, "Loading");
		DirectZob::GetInstance()->LoadScene(scenePath, sceneName);
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::Select))
	{
		std::string sceneName = "testGround.dzs";
		std::string scenePath = DirectZob::GetInstance()->GetResourcePath();
		hud->Print(ZobHUDManager::eHudUnit_ratio, 0.25f, 0.5f, 4, &color, "Loading");
		DirectZob::GetInstance()->LoadScene(scenePath, sceneName);
	}
}

TiXmlNode* ZobBehaviorMenu::SaveUnderNode(TiXmlNode* node)
{
	return NULL;
}

void ZobBehaviorMenu::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{

}