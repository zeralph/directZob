#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include "../Managers/ZobHUDManager.h"
#include <string>
#include <vector>

class ZobBehaviorMenu : public ZobBehavior
{
	friend class ZobBehaviorFactory;
	public:

		enum eMenuAction
		{
			eAction_none=0,
			eAction_Load,
			eAction_exit,
		};

		struct MenuEntry
		{
			std::string name;
			std::string file;
			eMenuAction action;
		};

		~ZobBehaviorMenu() override;
		void					Init() override;
		void					PreUpdate(float dt) override;
		void					UpdateAfterObject(float dt) override {}
		void					UpdateBeforeObject(float dt) override;
		void					PostUpdate() override {}
		void					EditorUpdate() override;
		void					DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorMenu(ZobObject* zobObject, bool bEditorZobBehavior);
		void					LoadMaterialInternal();
		ZobHUDManager::eHudUnit m_unit;
		float m_x;
		float m_y;
		float m_w;
		float m_h;
		float m_fontSize;
		const char* m_fontName;
		ZobColor m_color;
		ZobFilePath m_texture;
		const ZobMaterial* m_image;
		std::string m_text;
		std::vector<MenuEntry>	m_menuEntries;
		int						m_menuIndex;
};