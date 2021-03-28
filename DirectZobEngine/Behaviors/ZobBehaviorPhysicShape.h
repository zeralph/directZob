#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#include <string>
#include <vector>

class ZobBehaviorPhysicShape : public ZobBehavior
{
	friend class ZobBehaviorFactory;
	public:

		~ZobBehaviorPhysicShape() override;
		void		Init() override;
		void		PreUpdate() override;
		void		Update(float dt) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorPhysicShape(ZobObject* zobObject, TiXmlElement* node);
};