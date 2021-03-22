#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"

class ZobBehaviorMenu : public ZobBehavior
{
	friend class ZobBehaviorFactory;
	public:
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
};