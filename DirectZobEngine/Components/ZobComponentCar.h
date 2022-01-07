#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobComponent.h"
#include "ZobComponentSprite.h"
#undef min
#undef max
#include <reactphysics3d/reactphysics3d.h>

class Zobobject;
class ZobComponentCar : public ZobComponent
{

	friend class ZobComponentFactory;
	public:

		class GroundRaycastClass : public reactphysics3d::RaycastCallback {
		public:

			virtual reactphysics3d::decimal notifyRaycastHit(const reactphysics3d::RaycastInfo& info);
			ZobComponentCar* Component;
		};

		enum eCarType
		{
			eCarType_prout = 0,
			eCarType_pouet,
			eCarType_truc,
		};

		~ZobComponentCar() override;
		void		Init(DirectZobType::sceneLoadingCallback cb) override;
		void		PreUpdate(float dt) override;
		void		PostUpdate() override {}
		void		EditorUpdate() override;
		void		UpdateBeforeObject(float dt) override;
		void		UpdateAfterObject(float dt) override {}
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobComponentCar(ZobObject* zobObject);
		void CheckGroundCollisions();
		void CheckEnvironmentCollision();
		void UpdateInputs(float dt);
		void UpdateWheels(float dt);
		ZobObject* m_frontLeftWheel;
		ZobObject* m_frontRightWheel;
		ZobObject* m_rearLeftWheel;
		ZobObject* m_rearRightWheel;
		ZobVector3 m_lastGroundPosition;
		ZobVector3 m_lastGroundNormal;
		ZobVector3 m_lastCollPosition;
		ZobVector3 m_lastCollLocalContact;
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
		float m_steeringRotationSpeedRS;
		float m_maxSteeringAngle;
		ZobVector3 m_velocityWorld;
		ZobVector3 m_accelerationWorld;
		float m_speed_ms;
		bool m_handBrake;
		bool m_goingRear;
		float m_mass;
		float m_maxGrip;	/* maximum (normalised) friction force, =diameter of friction circle */
		float m_drag;		 		/* factor for air resistance (drag) 	*/
		float m_resistance;			/* factor for rolling resistance */
		float m_ca_r;			/* cornering stiffness */
		float m_ca_f;			/* cornering stiffness */

		eCarType m_carType;

		float m_wheelRotationAngle;
		ZobComponentSprite* m_test;
};