#pragma once

#undef min
#undef max
#undef None

#include "ZobComponentPhysicShape.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobComponentPhysicBox : public ZobComponentPhysicShape
{
	friend class ZobComponentFactory;
	public:


								~ZobComponentPhysicBox() override;
		void					Init(DirectZobType::sceneLoadingCallback cb) override;
		void					EditorUpdate() override;
	private:	
								ZobComponentPhysicBox(ZobObject* zobObject);

		BoxShape* m_boxShape;
		ZobVector3 m_halfExtends;
};