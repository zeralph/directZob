#pragma once

#undef min
#undef max
#undef None

#include "../../Types.h"
#include "../tinyxml/tinyxml.h"
#include "../ZobBehavior.h"
#include "../../ZobPhysic/ZobPhysicComponent.h"
#include <reactphysics3d/reactphysics3d.h>
#include <string>
#include <vector>

class ZobBehaviorPhysicShape : public ZobBehavior
{
	friend class ZobBehaviorFactory;
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

								~ZobBehaviorPhysicShape() override;
		void					Init() override;
		void					PreUpdate() override;
		void					Update(float dt) override;
		void					EditorUpdate() override;
		void					DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;

	protected:	
								ZobBehaviorPhysicShape(ZobObject* zobObject);
		void					AddColliderInternal(CollisionShape* c);
		virtual void			RemoveCollider();

		Collider* m_collider;
		int m_layers;
		bool m_isTrigger;
		ZobVector3 m_localPostion;
		std::string m_test;
		//ZobVector3 m_localRotation;
		bool m_bUpdateSize;
		float m_bounciness;
		float m_frictionCoeff;
		float m_massDensity;
		float m_rollingResistance;
};