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
		void CheckCollisions();
		float m_motorForce;
		float m_breakForce;
		float m_lineaVelocityMS;
		float m_mass;
		float m_heightAboveGround;
		bool m_drifting;		
		ZobVector3 m_direction;
		ZobVector3 m_lastGroundPosition;
};