#pragma once
#include "../../Types.h"
#include "../../tinyxml/tinyxml.h"
#include "../ZobComponent.h"
#include "../../Managers/ZobHUDManager.h"
#include <string>
#include <vector>

class ZobComponentText : public ZobComponent
{
	friend class ZobComponentFactory;
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

		~ZobComponentText() override;
		void					Init() override;
		void					PreUpdate(float dt) override;
		void					UpdateAfterObject(float dt) override {}
		void					UpdateBeforeObject(float dt) override;
		void					PostUpdate() override {}
		void					EditorUpdate() override;
		void					DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobComponentText(ZobObject* zobObject, bool bEditorZobComponent);
		void					LoadFontInternal();
		ZobHUDManager::eHudUnit m_unit;
		float m_x;
		float m_y;
		float m_w;
		float m_h;
		float m_fontSize;
		const char* m_fontName;
		ZobColor m_color;
		ZobFilePath m_fontXml;
		ZobFilePath m_fontImage;
		const ZobFont* m_font;
		float m_z;
		bool m_visible;
		std::string m_text;
};