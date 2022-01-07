#pragma once

#undef min
#undef max
#undef None

#include "../../Types.h"
#include "../tinyxml/tinyxml.h"
#include "../ZobComponent.h"
#include "../../ZobPhysic/ZobPhysicComponent.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobComponentPhysicShape : public ZobComponent
{
	friend class ZobComponentFactory;
	public:

		enum eShapeType
		{
			eShapeType_none = 0,
			eShapeType_sphere,
			eShapeType_capsule,
			eShapeType_box,
			eShapeType_convexMesh,
			__eShapeType_MAX__
		};

								~ZobComponentPhysicShape() override;
		void					Init(DirectZobType::sceneLoadingCallback cb) override;
		void					PreUpdate(float dt) override;
		void					UpdateBeforeObject(float dt) override {}
		void					UpdateAfterObject(float dt) override {}
		void					PostUpdate() override;
		void					EditorUpdate() override;
		void					DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;

	protected:	
								ZobComponentPhysicShape(ZobObject* zobObject);
		void					AddColliderInternal(CollisionShape* c);
		virtual void			RemoveCollider();

		Collider* m_collider;
		int m_layers;
		bool m_isTrigger;
		ZobVector3 m_localPostion;
		bool m_bUpdateSize;
		bool m_isInit;
		//float m_bounciness;
		//float m_frictionCoeff;
		//float m_massDensity;
		//float m_rollingResistance;
};