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
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node);
		void CheckGroundCollisions();
		void CheckEnvironmentCollision();
		void UpdateInputs();

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
};