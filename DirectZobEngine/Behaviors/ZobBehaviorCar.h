#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>

class ZobBehaviorCar : public ZobBehavior
{

	friend class ZobBehaviorFactory;
	public:

		class GroundRaycastClass : public reactphysics3d::RaycastCallback {
		public:

			virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info);
			ZobBehaviorCar* behavior;
		};

		enum eCarType
		{
			eCarType_prout = 0,
			eCarType_pouet,
			eCarType_truc,
		};

		~ZobBehaviorCar() override;
		void		Init() override;
		void		PreUpdate() override;
		void		EditorUpdate() override;
		void		Update(float dt) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorCar(ZobObject* zobObject);
		void CheckGroundCollisions();
		void CheckEnvironmentCollision();
		void UpdateInputs(float dt);

		ZobVector3 m_lastGroundPosition;
		ZobVector3 m_lastGroundNormal;

		ZobVector3 m_lastCollPosition;
		ZobVector3 m_lastCollDirection;
		ZobVector3 m_lastCollNormal;
		ZobVector3 m_lastCollRebound;
		bool m_hadCollision;
		float m_heightAboveGround;
		float m_angle;
		float m_angularvelocity;
		float m_steerangle;
		float m_throttle;
		float m_brake;
		float m_inertia;
		ZobVector3 m_velocityWorld;
		ZobVector3 m_accelerationWorld;
		float m_speed_ms;
		bool m_handBrake;

		float m_mass;
		float m_maxGrip;	/* maximum (normalised) friction force, =diameter of friction circle */
		float m_drag;		 		/* factor for air resistance (drag) 	*/
		float m_resistance;			/* factor for rolling resistance */
		float m_ca_r;			/* cornering stiffness */
		float m_ca_f;			/* cornering stiffness */

		eCarType m_carType;

};