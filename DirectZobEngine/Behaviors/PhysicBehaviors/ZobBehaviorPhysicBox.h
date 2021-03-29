#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehaviorPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobBehaviorPhysicBox : public ZobBehaviorPhysicShape
{
	friend class ZobBehaviorFactory;
	public:


								~ZobBehaviorPhysicBox() override;
		void					Init() override;
		void					EditorUpdate() override;
	private:	
								ZobBehaviorPhysicBox(ZobObject* zobObject);

		BoxShape* m_boxShape;
		ZobVector3 m_halfExtends;
};