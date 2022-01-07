#pragma once

#undef min
#undef max
#undef None

#include "../tinyxml/tinyxml.h"
#include "ZobComponentPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobComponentPhysicCapsule : public ZobComponentPhysicShape
{
	friend class ZobComponentFactory;
	public:

								~ZobComponentPhysicCapsule() override;
		void					Init(DirectZobType::sceneLoadingCallback cb) override;
		void					EditorUpdate() override;
	private:	
								ZobComponentPhysicCapsule(ZobObject* zobObject);


		float m_radius;
		float m_height;
		CapsuleShape* m_capsuleShape;
};