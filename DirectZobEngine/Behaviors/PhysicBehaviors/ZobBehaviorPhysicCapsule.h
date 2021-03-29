#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehaviorPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobBehaviorPhysicCapsule : public ZobBehaviorPhysicShape
{
	friend class ZobBehaviorFactory;
	public:

								~ZobBehaviorPhysicCapsule() override;
		void					Init() override;
		void					EditorUpdate() override;
	private:	
								ZobBehaviorPhysicCapsule(ZobObject* zobObject);


		float m_radius;
		float m_height;
		CapsuleShape* m_capsuleShape;
};