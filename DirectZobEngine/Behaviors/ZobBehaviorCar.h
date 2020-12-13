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
		void		PreUpdate() override;
		void		Update(float dt) override;
		void		DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const override;
	private:	
		ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node);

		float m_rollingTorquePower ;
		float m_steeringTorque;
		float m_rollingTorqueMaxPower;
		float m_currentMaxTorqueApplicable;
		float m_motorForce;
		float m_breakForce;
		float m_airFriction;
		ZobVector3 m_frontWheelPosition;
		ZobVector3 m_rearWheelPosition;
		bool m_bCcanChangeCamera;
		bool m_drifting;
};