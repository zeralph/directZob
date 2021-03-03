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
	m_motorForce = 5.0f;
	m_breakForce = 20.0f;
	m_lineaVelocityMS = 0.0f;
	m_steeringMaxAngle = 10.0f;
	m_drifting = false;
	m_mass = 750.0f;
	m_bRear = false;
	m_heightAboveGround = 0.9f;
	m_direction = ZobVector3(0, 0, 0);
	m_lastGroundPosition = ZobVector3(0, 0, 0);
	m_lastGroundNormal = ZobVector3(0, 1, 0);

	m_lastCollPosition = ZobVector3(0, 0, 0);
	m_lastCollDirection= ZobVector3(0, 0, 0);
	m_lastCollRebound  = ZobVector3(0, 0, 0);
	m_lastCollNormal   = ZobVector3(0, 0, 0);
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Init()
{

}

void ZobBehaviorCar::CheckEnvironmentCollision()
{
}

void ZobBehaviorCar::CheckGroundCollisions()
{
	DirectZob* directZob = DirectZob::GetInstance();
	RigidBody* rb = m_zobObject->m_physicComponent->GetRigicBody();
	Vector3 p = Vector3(rb->getTransform().getPosition());
	Vector3 e = Vector3(rb->getTransform().getPosition());
	p.y += 1000.0f;
	e.y -= 1000.0f;
	reactphysics3d::Ray ray(p, e);
	reactphysics3d::RaycastInfo info;
	const std::vector<ZobPhysicComponent*>*  bodies = DirectZob::GetInstance()->GetPhysicsEngine()->GetBodies();
	std::vector<ZobPhysicComponent*>::const_iterator iter;
	bool bHit = false;
	for (iter = bodies->begin(); iter != bodies->end(); iter++)
	{
		RigidBody* rb2 = (*iter)->GetRigicBody();
		if (((*iter)->GetLayers() & ZobPhysicComponent::eLayer_ground) && rb2 != rb)
		{
			if (rb2->raycast(ray, info))
			{
				m_lastGroundPosition.x = info.worldPoint.x;
				m_lastGroundPosition.y = info.worldPoint.y;
				m_lastGroundPosition.z = info.worldPoint.z;
				m_lastGroundNormal.x = info.worldNormal.x;
				m_lastGroundNormal.y = info.worldNormal.y;
				m_lastGroundNormal.z = info.worldNormal.z;
				//_lastGroundNormal.Mul(-1.0f);
				bHit = true;
				//break;
			}
		}
	}
	if (!bHit)
	{
		m_lastGroundNormal = ZobVector3(0, 1, 0);
		m_lastGroundPosition = m_zobObject->GetWorldPosition();
		m_lastGroundPosition.y = 0;
	}
}

void ZobBehaviorCar::Update(float dt)
{
	CheckGroundCollisions();
	CheckEnvironmentCollision();
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		float acc = 0.0f;
		float brk = 0.0f;
		float dir = 0.0f;
		acc = inputMap->GetFloat(ZobInputManager::RightShoulder) * m_motorForce;
		if (m_bRear)
		{
			acc *= 10.0f;
		}
		brk = inputMap->GetFloat(ZobInputManager::LeftShoulder) * m_breakForce;
		if (fabsf(m_lineaVelocityMS) > 0.5f)
		{
			dir = inputMap->GetFloat(ZobInputManager::LeftStickX) * m_steeringMaxAngle / (m_lineaVelocityMS/2.0f);
		}
		ZobVector3 pos = m_zobObject->GetWorldPosition();
		m_direction = m_zobObject->GetForward();
		ZobVector3 collRebound = ZobVector3(0, 0, 0);
		ZobPhysicComponent* zc = m_zobObject->m_physicComponent;
		ZobPhysicComponent::collision* coll = zc->GetLastCollision();
		if (!coll->handled)
		{
			if ((coll->collisionLayer & ZobPhysicComponent::eLayer_wall))
			{
				m_lastCollPosition = coll->collisionWorldPosition;
				m_lastCollDirection = coll->collisionWorldDirection;
				m_lastCollNormal = coll->collisionWorldNormal;

				m_lineaVelocityMS /= 2.0f;
				collRebound = pos - coll->collisionWorldPosition;
				collRebound.y = 0;
				collRebound.Normalize();
				ZobVector3 collNormal = coll->collisionWorldNormal;
				collNormal.y = 0;
				collNormal.Mul(-1.0f);
				collNormal.Normalize();

				m_lastCollRebound = collRebound;
				float d = -ZobVector3::Dot(&m_lastCollRebound, &collNormal);
				float angle = M_PI / 2.0f;// *d / fabsf(d);
				float sinA = sinf(angle);
				float cosA = cosf(angle);
				m_lastCollRebound.x = m_lastCollRebound.x * cosA - m_lastCollRebound.z * sinA;
				m_lastCollRebound.z = m_lastCollRebound.x * sinA + m_lastCollRebound.z * cosA;
				m_lastCollRebound.Normalize();
				//collRebound.Mul(fminf(fmaxf(1.0f, m_lineaVelocityMS), 5.0f));
				m_lastCollRebound.Mul(10);

				collRebound.Mul(fminf(fmaxf(1.0f, m_lineaVelocityMS), 5.0f));
				pos.Add(&collRebound);
			}
			else if ((coll->collisionLayer & ZobPhysicComponent::eLayer_objects))
			{
				//other cars
			}
			coll->Reset();
		}
		//else
		{
			float f = (acc - brk);
			m_lineaVelocityMS += f * dt;
			if (m_lineaVelocityMS < 0.0f)
			{
				m_lineaVelocityMS = fmaxf(m_lineaVelocityMS, -10.0f);
				m_bRear = true;
			}
			else
			{
				m_bRear = false;
			}
			if (fabsf(m_lineaVelocityMS) > 1.0f)
			{
				float p = fabsf(m_lineaVelocityMS) * dt;
				m_direction.y = 0.0f;
				m_direction.Normalize();
				float angle = DEG_TO_RAD(dir);
				//angle = M_PI / 2.0f;
				float sinA = sinf(angle);
				float cosA = cosf(angle);
				m_direction.x = m_direction.x * cosA - m_direction.z * sinA;
				m_direction.z = m_direction.x * sinA + m_direction.z * cosA;
				m_direction.Normalize();
				m_direction.Mul(p);
				if (m_bRear)
				{
					pos.Sub(&m_direction);
				}
				else
				{
					pos.Add(&m_direction);
				}
			}
		}
		
		m_zobObject->SetWorldPosition(pos.x, m_lastGroundPosition.y + m_heightAboveGround, pos.z);
		m_lastGroundNormal.Normalize();
		m_direction.Normalize();
		ZobVector3 forward = m_direction;
		ZobVector3 up = m_zobObject->GetUp();
		up.x = up.x * 0.7f + m_lastGroundNormal.x * 0.3f;
		up.y = up.y * 0.7f + m_lastGroundNormal.y * 0.3f;
		up.z = up.z * 0.7f + m_lastGroundNormal.z * 0.3f;
		up.Normalize();
		ZobVector3 left = ZobVector3::Cross(&forward, &up);
		left.Normalize();
		left.Mul(-1.0f);

		
		forward = ZobVector3::Cross(&left, &up);
		forward.Normalize();
		m_zobObject->LookAt(&forward, &left, &up, false);
		uint color = 0xFFFF00;
		if (m_drifting)
		{
			color = 0xFF0000;
		}
		Text2D* m_textManager = directZob->GetTextManager();
		ZobVector3 c = ZobVector3(255.0f, 1.0f, 0.1f);
		DirectZob::GetInstance()->GetHudManager()->Print(0.5f, 0.9f, 0.05f, 0.05f, &c, "%.2f kmh", MS_TO_KMH(m_lineaVelocityMS));
		//m_textManager->Print(10, 60, color, "speed            : %.2f km/h", MS_TO_KMH(m_lineaVelocityMS));
		m_textManager->Print(10, 75, color, "angle            : %.2f / %.2f", dir, DEG_TO_RAD(dir));
	}

	//update current camera
	if (inputMap->GetBoolIsNew(ZobInputManager::NextCamera))
	{
		directZob->GetCameraManager()->SwitchToNextAvailableCamera();
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

		p = m_lastCollPosition;
		v = m_lastCollDirection;
		v.Mul(10.0f);
		v = v + p;
		e->QueueLine(camera, &p, &v, 0x00FFFF, true, true);
		ZobMatrix4x4 m = ZobMatrix4x4();
		m.AddTranslation(m_lastCollPosition);
		e->QueueSphere(camera, &m, 1, 0xFF0000, false, false);

		p = m_lastCollPosition;
		v = m_lastCollRebound;
		v = v + p;
		v.Mul(10.0f);
		e->QueueLine(camera, &p, &v, 0xFF00FF, true, true);

		p = m_lastCollPosition;
		v = m_lastCollNormal;
		v = v + p;
		v.Mul(10.0f);
		e->QueueLine(camera, &p, &v, 0xFFFF00, true, true);
	}
}