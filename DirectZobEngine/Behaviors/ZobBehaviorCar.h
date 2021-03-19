#pragma once
#include "../Types.h"
#include "../tinyxml/tinyxml.h"
#include "ZobBehavior.h"

class ZobBehaviorCar : public ZobBehavior
{
	friend class ZobBehaviorFactory;
	public:
		~ZobBehaviorCar() override;
		const char* GetBehaviorTypeStr() override { return "car"; }
		TiXmlNode*	SaveUnderNode(TiXmlNode* node) override;
		void		Init() override;
		void		PreUpdate() override;
		void		Update(float dt) override;
		void		Update_old(float dt);
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node);
		void CheckGroundCollisions();
		void CheckEnvironmentCollision();
		void UpdateInputs();
		float m_motorForce;
		float m_breakForce;
		float m_lineaVelocityMS;
		float m_mass;
		float m_heightAboveGround;
		bool m_drifting;	
		float m_steeringMaxAngle;
		bool m_bRear;
		ZobVector3 m_direction;
		ZobVector3 m_lastGroundPosition;
		ZobVector3 m_lastGroundNormal;

		ZobVector3 m_lastCollPosition;
		ZobVector3 m_lastCollDirection;
		ZobVector3 m_lastCollNormal;
		ZobVector3 m_lastCollRebound;

		float m_angle;
		float m_angularvelocity;
		float m_steerangle;
		float m_throttle;
		float m_brake;
		float m_inertia;
		ZobVector3 m_velocityWorld;
		ZobVector3 m_accelerationWorld;
};