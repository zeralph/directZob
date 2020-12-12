#include "ZobBehaviorCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"

ZobBehaviorCar::~ZobBehaviorCar()
{

}

ZobBehaviorCar::ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node) : ZobBehavior(zobObject, node)
{
	m_type = eBehavior_car;
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Update()
{
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();

		ZobVector3 fw = m_zobObject->GetForward();
		ZobVector3 lt = m_zobObject->GetLeft();
		Vector3 forward = rb->getWorldVector(Vector3(0, 0, 1));
		Vector3 left = rb->getWorldVector(Vector3(1, 0, 0));
		Vector3 linearVelocity = rb->getLinearVelocity();
		Vector3 linearVelocityLocal = rb->getLocalVector(linearVelocity);
		Vector3 angulaVelocity = rb->getAngularVelocity();
		Vector3 angulaVelocityLocal = rb->getLocalVector(angulaVelocity);
		
		float frictioncoeff = rb->getCollider(0)->getMaterial().getFrictionCoefficient();
		const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
		float acc = inputMap->GetFloat(ZobInputManager::RightShoulder) * 40.0f;
		float brk = 0.0f;
		float dir = 0.0f;
		if (linearVelocityLocal.z > 0.1f)
		{
			dir = -inputMap->GetFloat(ZobInputManager::LeftStickX) * 1.0f;
			brk = -inputMap->GetFloat(ZobInputManager::LeftShoulder) * 20.0f;
		}
		Vector3 acceleration = forward * acc;
		Vector3 breaking = forward * brk;
		Vector3 velocityFriction = forward * (-linearVelocityLocal.z * 0.8f);
		rb->applyForceToCenterOfMass(acceleration);// , Vector3(0, 0, 1));
		rb->applyForceToCenterOfMass(breaking);// , Vector3(0, 0, -1));
		rb->applyForceToCenterOfMass(velocityFriction);

		if (dir != 0.0f && fabs(angulaVelocityLocal.y) < 0.5f)
		{
			//todo; dir torque decrease with speed
			Vector3 str = Vector3(0, dir, 0);
			rb->applyTorque(str);
		}
		else
		{
			angulaVelocityLocal.y *= 0.9f;
			angulaVelocity = rb->getWorldVector(angulaVelocityLocal);
			rb->setAngularVelocity(angulaVelocity);
		}

		//wheel friction
		linearVelocityLocal.x = 0.0f;
		linearVelocity = rb->getWorldVector(linearVelocityLocal);
		rb->setLinearVelocity(linearVelocity);
		
		//stop vehicle if no force and no speed
		if ((linearVelocityLocal.z < 0.1f) && (acc == 0.0f))
		{
			rb->setLinearVelocity(Vector3(0, linearVelocity.y, 0));
		}

		float kmh = fabs(linearVelocityLocal.z * 3600.0f / 1000.0f);
		Text2D* m_textManager = directZob->GetTextManager();
		m_textManager->Print(10, 60, 4, 0xFFFF00, "speed            : %.2f km/h", kmh);
		m_textManager->Print(10, 75, 4, 0xFFFF00, "angular velocity : %.2f", fabs(angulaVelocity.y));
	}
}

TiXmlNode* ZobBehaviorCar::SaveUnderNode(TiXmlNode* node)
{
	TiXmlElement m = TiXmlElement("Behavior");
	m.SetAttribute("type", (int)eBehavior_car);
	node->InsertEndChild(m);
	return &m;
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