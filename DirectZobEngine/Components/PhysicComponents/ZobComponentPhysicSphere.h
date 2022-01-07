#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobComponentPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>


class ZobComponentPhysicSphere : public ZobComponentPhysicShape
{
	friend class ZobComponentFactory;
	public:

								~ZobComponentPhysicSphere() override;
		void					Init(DirectZobType::sceneLoadingCallback cb) override;
		void					EditorUpdate() override;
	private:	
								ZobComponentPhysicSphere(ZobObject* zobObject);

		float m_radius;
		SphereShape* m_sphereShape;
};