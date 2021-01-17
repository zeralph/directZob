#include "ZobBehaviorCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"

#define MS_TO_KMH(a) a * 3.6f 
#define KMH_TO_MS(a) a / 3.6f 

class ZobPhysicsRaycastCallback : public RaycastCallback {

public:

	virtual decimal notifyRaycastHit(const RaycastInfo& info)
	{
		m_hit.x = info.worldPoint.x;
		m_hit.y = info.worldPoint.y;
		m_hit.z = info.worldPoint.z;
		m_body = info.body;
		m_collider = info.collider;
		return decimal(1.0);
	}
	ZobVector3 m_hit;
	CollisionBody* m_body;
	Collider* m_collider;
};

ZobBehaviorCar::~ZobBehaviorCar()
{

}

ZobBehaviorCar::ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node) : ZobBehavior(zobObject, node)
{
	m_type = eBehavior_car;
	m_motorForce = 5.0f;
	m_breakForce = 10.0f;
	m_lineaVelocityMS = 0.0f;
	m_drifting = false;
	m_mass = 750.0f;
	m_heightAboveGround = 1.0f;
	m_direction = ZobVector3(0, 0, 0);
	m_lastGroundPosition = ZobVector3(0, 0, 0);
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Init()
{

}

void ZobBehaviorCar::CheckCollisions()
{
	DirectZob* directZob = DirectZob::GetInstance();
	RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();
	Vector3 p = Vector3(rb->getTransform().getPosition());
	Vector3 e = Vector3(rb->getTransform().getPosition());
	p.y -= 1000.0f;
	e.y += 1000.0f;
	reactphysics3d::Ray ray(p, e);
	ZobPhysicsRaycastCallback callbackObject;
	directZob->GetPhysicsEngine()->GetWorld()->raycast(ray, &callbackObject);
	m_lastGroundPosition = callbackObject.m_hit;
}

void ZobBehaviorCar::Update(float dt)
{
	CheckCollisions();
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		float acc = 0.0f;
		float brk = 0.0f;
		float dir = 0.0f;
		acc = inputMap->GetFloat(ZobInputManager::RightShoulder) * m_motorForce;
		if (m_lineaVelocityMS > 0.1f)
		{
			dir = inputMap->GetFloat(ZobInputManager::LeftStickX) * 30.0f / m_lineaVelocityMS;
			brk = inputMap->GetFloat(ZobInputManager::LeftShoulder) * m_breakForce;
		}
		
		float f = (acc-brk);
		m_lineaVelocityMS += f * dt;
		float p = m_lineaVelocityMS * dt;
		m_direction = m_zobObject->GetForward();	
		m_direction.y = 0.0f;
		m_direction.Normalize();

		float angle = DEG_TO_RAD(dir);
		//angle = M_PI / 2.0f;
		float sinA = sinf(angle);
		float cosA = cosf(angle);

		m_direction.x = m_direction.x * cosA - m_direction.z * sinA;
		m_direction.z = m_direction.x * sinA + m_direction.z * cosA;
		m_direction.Normalize();
		ZobVector3 forward = m_direction;
		m_direction.Mul(p);
		ZobVector3 pos = m_zobObject->GetWorldPosition();
		pos.Add(&m_direction);
		m_zobObject->SetWorldPosition(pos.x, m_lastGroundPosition.y + m_heightAboveGround, pos.z);
		
		
		ZobVector3 left = ZobVector3::Cross(&forward, &ZobVector3::Vector3Y);
		left.Normalize();
		left.Mul(-1.0f);
		ZobVector3 up = ZobVector3::Cross(&forward, &left);
		up.Normalize();
		m_zobObject->LookAt(&forward, &left, &up, false);

		uint color = 0xFFFF00;
		if (m_drifting)
		{
			color = 0xFF0000;
		}
		Text2D* m_textManager = directZob->GetTextManager();
		m_textManager->Print(10, 60, 4, color, "speed            : %.2f km/h", MS_TO_KMH(m_lineaVelocityMS));
		m_textManager->Print(10, 75, 4, color, "angle            : %.2f / %.2f", dir, DEG_TO_RAD(dir));
		//m_textManager->Print(10, 90, 4, color, "rolling torque : %.2f", fabs(rollingTorque));
		//m_textManager->Print(10, 105, 4, color, "drift : %.2f", fabs(linearVelocityLocal.x));
	}

	//update current camera
	if (inputMap->GetBoolIsNew(ZobInputManager::buttonA))
	{
		directZob->GetCameraManager()->SwitchToNextAvailableCamera();
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::buttonY))
	{
		directZob->GetEngine()->DrawGizmos(!directZob->GetEngine()->DrawGizmos());
	}
	if (inputMap->GetBoolIsNew(ZobInputManager::buttonX))
	{
		directZob->StopPhysic(true);
		directZob->StartPhysic();
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
		Engine* e = DirectZob::GetInstance()->GetEngine();
		ZobVector3 p = m_zobObject->GetWorldPosition();
		ZobVector3 v = m_direction;
		v.Mul(10.0f);
		v.x += p.x;
		v.y += p.y;
		v.z += p.z;
		e->QueueLine(camera, &p, &v, 0xFFFFFF, true, true);
	}
}