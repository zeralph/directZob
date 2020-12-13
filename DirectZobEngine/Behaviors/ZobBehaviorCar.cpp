#include "ZobBehaviorCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"

#define MS_TO_KMH(a) a * 3.6f 
#define KMH_TO_MS(a) a / 3.6f 

ZobBehaviorCar::~ZobBehaviorCar()
{

}

ZobBehaviorCar::ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node) : ZobBehavior(zobObject, node)
{
	m_type = eBehavior_car;
	m_frontWheelPosition = ZobVector3(0, 0, 4);
	m_rearWheelPosition = ZobVector3(0, 0, -2);
	m_rollingTorquePower = 1.0f;
	m_steeringTorque = 1.0f * 60.0f;
	m_rollingTorqueMaxPower = 30.0f;
	m_currentMaxTorqueApplicable = 40.0f;
	m_motorForce = 10.0f * 60.0f;
	m_breakForce = 10.0f * 60.0f;
	m_airFriction = 0.005f * 60.0f;
	m_drifting = false;
	RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();
	Vector3 centerOfMass = rb->getLocalCenterOfMass();
	rb->setLocalCenterOfMass(Vector3(0, 0, -2.5));
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Update(float dt)
{
	//dt = 1.0f / 60.0f;
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();
		Vector3 centerOfMass = rb->getLocalCenterOfMass();
		//rb->setLocalCenterOfMass(Vector3(0, 0, -2));
		ZobVector3 fw = m_zobObject->GetForward();
		ZobVector3 lt = m_zobObject->GetLeft();
		Vector3 forward = rb->getWorldVector(Vector3(0, 0, 1));
		Vector3 left = rb->getWorldVector(Vector3(1, 0, 0));
		Vector3 linearVelocity = rb->getLinearVelocity();
		Vector3 linearVelocityLocal = rb->getLocalVector(linearVelocity);
		Vector3 angulaVelocity = rb->getAngularVelocity();
		Vector3 angulaVelocityLocal = rb->getLocalVector(angulaVelocity);
		Vector3 rearWheelPosition = Vector3(m_rearWheelPosition.x, m_rearWheelPosition.y, m_rearWheelPosition.z);
		Vector3 frontWheelPosition = Vector3(m_frontWheelPosition.x, m_frontWheelPosition.y, m_frontWheelPosition.z);
		float speedKMH = MS_TO_KMH(fabs(linearVelocityLocal.z));
		float rollingTorque = 0.0f;
		float frictioncoeff = rb->getCollider(0)->getMaterial().getFrictionCoefficient();
		float currentMaxTorque = m_currentMaxTorqueApplicable * (1.0f - fmaxf(0, speedKMH-50.0f) / 170.0f) * dt;
		float acc = 0.0f;
		float brk = 0.0f;
		float dir = 0.0f;

		acc = inputMap->GetFloat(ZobInputManager::RightShoulder) * m_motorForce * dt;
		if (linearVelocityLocal.z > 0.1f)
		{
			dir = -inputMap->GetFloat(ZobInputManager::LeftStickX) * m_steeringTorque * dt;
			brk = -inputMap->GetFloat(ZobInputManager::LeftShoulder) * m_breakForce * dt;
			
			
		}
		Vector3 acceleration = forward * acc;
		Vector3 breaking = forward * brk;
		Vector3 velocityFriction = forward * (-(linearVelocityLocal.z* linearVelocityLocal.z) * m_airFriction * dt);
		rb->applyForceAtLocalPosition(acceleration , rearWheelPosition);
		rb->applyForceAtLocalPosition(breaking, frontWheelPosition);
		rb->applyForceToCenterOfMass(velocityFriction);

		if (dir != 0.0f /*&& fabs(angulaVelocityLocal.y) < currentMaxTorque*/ && linearVelocityLocal.z > KMH_TO_MS(10.0f))
		{
			static bool bUseTorque = true;
			if (bUseTorque)
			{
				//todo; dir torque decrease with speed
				Vector3 str = Vector3(0, dir, 0);
				rb->applyTorque(str);
			}
			else
			{
				Vector3 str = Vector3(dir, 0, 0);
				str = rb->getWorldVector(str);
				rb->applyForceAtLocalPosition(str, frontWheelPosition);
			}
		}
		else
		{
			angulaVelocityLocal.y *= 0.5f;
			angulaVelocityLocal.y = 0.0f;
			angulaVelocity = rb->getWorldVector(angulaVelocityLocal);
			rb->setAngularVelocity(angulaVelocity);
		}

		//clamp torque
		angulaVelocity = rb->getAngularVelocity();
		angulaVelocityLocal = rb->getLocalVector(angulaVelocity);
		float f = fmaxf(-currentMaxTorque, fminf(angulaVelocityLocal.y, currentMaxTorque));
		m_drifting = m_drifting || (f != angulaVelocityLocal.y);
		angulaVelocityLocal.y = f;
		angulaVelocity = rb->getWorldVector(angulaVelocityLocal);
		rb->setAngularVelocity(angulaVelocity);

		//Rolling
		/*
		rollingTorque = copysignf(min(m_rollingTorqueMaxPower, m_rollingTorquePower * linearVelocityLocal.z), dir);
		Vector3 rolling = Vector3(0, 0, rollingTorque);
		rb->applyTorque(rolling);
		*/
		m_drifting = false;

		//wheel friction
		if (!m_drifting)
		{
			linearVelocityLocal.x = 0.0f;
			linearVelocity = rb->getWorldVector(linearVelocityLocal);
			rb->setLinearVelocity(linearVelocity);
		}
		else
		{
			m_drifting = !(linearVelocityLocal.x <= 0.01f);
			if (dir == 0.0f)
			{
				linearVelocityLocal.x /= (2.0f);
				linearVelocity = rb->getWorldVector(linearVelocityLocal);
				rb->setLinearVelocity(linearVelocity);
			}
		}
		//stop vehicle if no force and no speed
		if ((linearVelocityLocal.z < KMH_TO_MS(10)) && (acc == 0.0f))
		{
			rb->setLinearVelocity(Vector3(0, linearVelocity.y, 0));
		}
		uint color = 0xFFFF00;
		if (m_drifting)
		{
			color = 0xFF0000;
		}
		Text2D* m_textManager = directZob->GetTextManager();
		m_textManager->Print(10, 60, 4, color, "speed            : %.2f km/h", speedKMH);
		m_textManager->Print(10, 75, 4, color, "angular/max velocity : %.2f/%.2f", fabs(angulaVelocity.y), currentMaxTorque);
		m_textManager->Print(10, 90, 4, color, "rolling torque : %.2f", fabs(rollingTorque));
		m_textManager->Print(10, 105, 4, color, "drift : %.2f", fabs(linearVelocityLocal.x));
	}

	//update current camera
	if (inputMap->GetBool(ZobInputManager::buttonA))
	{
		if (m_bCcanChangeCamera)
		{
			m_bCcanChangeCamera = false;
			directZob->GetCameraManager()->SwitchToNextAvailableCamera();
		}
	}
	else
	{
		m_bCcanChangeCamera = true;
	}
}

TiXmlNode* ZobBehaviorCar::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement m = TiXmlElement("Behavior");
	m.SetAttribute("type", (int)eBehavior_car);
	node->InsertEndChild(m);
	return NULL;
}

void ZobBehaviorCar::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();
		Engine* e = DirectZob::GetInstance()->GetEngine();
		ZobVector3 p = m_zobObject->GetWorldPosition();
		ZobVector3 v = ZobVector3(rb->getLinearVelocity().x, rb->getLinearVelocity().y, rb->getLinearVelocity().z);
		v.x += p.x;
		v.y += p.y;
		v.z += p.z;
		e->QueueLine(camera, &p, &v, 0x0000FF, true, false);
	}
}