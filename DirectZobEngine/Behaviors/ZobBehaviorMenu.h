#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
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
		const char* GetBehaviorTypeStr() override { return "menu"; }
		TiXmlNode*	SaveUnderNode(TiXmlNode* node) override;
		void		Init() override;
		void		PreUpdate() override;
		void		Update(float dt) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorMenu(ZobObject* zobObject, TiXmlElement* node);
		float m_time;
		std::vector<MenuEntry> m_menuEntries;
		int m_menuIndex;
};