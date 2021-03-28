#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobBehaviorPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>


class ZobBehaviorPhysicSphere : public ZobBehaviorPhysicShape
{
	friend class ZobBehaviorFactory;
	public:

								~ZobBehaviorPhysicSphere() override;
		void					Init() override;
		void					EditorUpdate() override;
	private:	
								ZobBehaviorPhysicSphere(ZobObject* zobObject);

		float m_radius;
		SphereShape* m_sphereShape;
};