#include "ZobBehaviorCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"

#define MS_TO_KMH(a) a * 3.6f 
#define KMH_TO_MS(a) a / 3.6f 

#undef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)

#define	DRAG		5.0		 		/* factor for air resistance (drag) 	*/
#define	RESISTANCE	30.0			/* factor for rolling resistance */
#define CA_R		-5.20			/* cornering stiffness */
#define CA_F		-5.0			/* cornering stiffness */
#define MAX_GRIP	2.0				/* maximum (normalised) friction force, =diameter of friction circle */


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
	m_angle=0;
	m_angularvelocity=0;
	m_steerangle=0;
	m_throttle=0;
	m_brake=0;
	m_mass = 1500;
	m_inertia = 1500;
	m_velocityWorld = ZobVector3(0, 0, 0);
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

void ZobBehaviorCar::UpdateInputs()
{
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	float inputAcc = 0.0f;
	float inputBrk = 0.0f;
	float inputDir = 0.0f;
	inputAcc = inputMap->GetFloat(ZobInputManager::RightShoulder);
	inputBrk = inputMap->GetFloat(ZobInputManager::LeftShoulder);
	inputDir = inputMap->GetFloat(ZobInputManager::LeftStickX);
	if (inputDir < 0)
	{
		if (m_steerangle < M_PI / 4.0)
		{
			m_steerangle += M_PI / 32.0;
		}
	}
	else if (inputDir > 0)
	{
		if (m_steerangle > -M_PI / 4.0)
		{
			m_steerangle -= M_PI / 32.0;
		}
	}
	else
	{
//		m_steerangle = 0;
		//back to zero
	}
	if (inputAcc && m_throttle < 100)
	{
		m_throttle += 10;
	}
	if (!inputBrk && !inputAcc && m_throttle >= 10)
	{
		m_throttle -= 10;
	}
	if (inputBrk)
	{
		m_brake = 100;
		m_throttle = 0;
	}
	else
	{
		m_brake = 0;
	}
	//update current camera
	if (inputMap->GetBoolIsNew(ZobInputManager::NextCamera))
	{
		DirectZob::GetInstance()->GetCameraManager()->SwitchToNextAvailableCamera();
	}
}

void ZobBehaviorCar::Update(float dt)
{
	CheckGroundCollisions();
	CheckEnvironmentCollision();
	UpdateInputs();
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		double	rot_angle;
		double	sideslip;
		double	slipanglefront;
		double	slipanglerear;
		double	torque;
		double	angular_acceleration;
		double	sn, cs;
		double	yawspeed;
		double	weight;
		int		rear_slip;
		int		front_slip;
		ZobVector3 velocity, resistance;
		ZobVector3 flatf, flatr;
		ZobVector3 ftraction, force;
		ZobVector3 acceleration;
		sn = sin(m_angle);
		cs = cos(m_angle);

		if (m_steerangle != 0.0f)
		{
			int breakme = 1;
		}

		ZobVector3 worldPos = m_zobObject->GetWorldPosition();

		// SAE convention: x is to the front of the car, y is to the right, z is down

		//	bangz: Velocity of Car. Vlat and Vlong
		// transform velocity in world reference frame to velocity in car reference frame
		velocity.x = cs * m_velocityWorld.x + sn * m_velocityWorld.z;
		velocity.y = m_velocityWorld.y;
		velocity.z = -sn * m_velocityWorld.x + cs * m_velocityWorld.z;

		// Lateral force on wheels
		//	
			// Resulting velocity of the wheels as result of the yaw rate of the car body
			// v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
			// yawrate (ang.velocity) must be in rad/s
			//
		yawspeed = 2/*car->cartype->wheelbase*/ * 0.5 * m_angularvelocity;

		//bangz: velocity.z = fVLong_, velocity.x = fVLat_
		if (velocity.z == 0)		// TODO: fix singularity
			rot_angle = 0;
		else
			rot_angle = atan2(yawspeed, velocity.z);

		// Calculate the side slip angle of the car (a.k.a. beta)
		if (velocity.x == 0)		// TODO: fix singularity
			sideslip = 0;
		else
			sideslip = atan2(velocity.x, velocity.z);

		// Calculate slip angles for front and rear wheels (a.k.a. alpha)
		slipanglefront = sideslip + rot_angle - m_steerangle;
		slipanglerear = sideslip - rot_angle;

		// weight per axle = half car mass times 1G (=9.8m/s^2) 
		weight = m_mass * 9.8 * 0.5;

		// lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
		flatf.y = 0;
		flatf.z = 0;
		flatf.x = CA_F * slipanglefront;
		flatf.x = fmin(MAX_GRIP, flatf.x);
		flatf.x = fmax(-MAX_GRIP, flatf.x);
		flatf.x *= weight;
		if (front_slip)
			flatf.x *= 0.5;

		// lateral force on rear wheels
		flatr.y = 0;
		flatr.z = 0;
		flatr.x = CA_R * slipanglerear;
		flatr.x = fmin(MAX_GRIP, flatr.x);
		flatr.x = fmax(-MAX_GRIP, flatr.x);
		flatr.x *= weight;
		if (rear_slip)
			flatr.x *= 0.5;

		// longtitudinal force on rear wheels - very simple traction model
		ftraction.z = 100 * (m_throttle - m_brake * SGN(velocity.z));
		ftraction.x = 0;
		ftraction.y = 0;
		if (rear_slip)
			ftraction.z *= 0.5;

		// Forces and torque on body

		// drag and rolling resistance
		resistance.x = 0;// -(RESISTANCE * velocity.x + DRAG * velocity.x * abs(velocity.x));
		resistance.y = 0;// -(RESISTANCE * velocity.y + DRAG * velocity.y * abs(velocity.y));
		resistance.z = 0;// -(RESISTANCE * velocity.z + DRAG * velocity.z * abs(velocity.z));

		// sum forces
		force.x = ftraction.x + cos(m_steerangle) * flatf.x + flatr.x + resistance.x;
		force.y = 0;
		force.z = ftraction.z + sin(m_steerangle) * flatf.z + flatr.z + resistance.z;
		

		// torque on body from lateral forces
		torque = /*car->cartype->b*/1 * flatf.x - /*car->cartype->c*/1 * flatr.x;

		// Acceleration

			// Newton F = m.a, therefore a = F/m
		acceleration.x = force.x / m_mass;
		acceleration.y = 0; //9.81
		acceleration.z = force.z / m_mass;
		

		angular_acceleration = torque / m_inertia;

		// Velocity and position

			// transform acceleration from car reference frame to world reference frame
		m_accelerationWorld.z = cs * acceleration.z + sn * acceleration.x;
		m_accelerationWorld.y = 0;
		m_accelerationWorld.x = -sn * acceleration.z + cs * acceleration.x;

		// velocity is integrated acceleration
		//
		m_velocityWorld.x += dt * m_accelerationWorld.x;
		m_velocityWorld.y += dt * m_accelerationWorld.y;
		m_velocityWorld.z += dt * m_accelerationWorld.z;

		// position is integrated velocity
		//
		ZobVector3 dp = worldPos;
		worldPos.x += dt * m_velocityWorld.x;
		worldPos.z += dt * m_velocityWorld.z;

		dp.x = worldPos.x - dp.x;
		dp.y = 0;// worldPos.x - dp.y;
		dp.z = worldPos.x - dp.z;

		// Angular velocity and heading

			// integrate angular acceleration to get angular velocity
			//
		m_angularvelocity += dt * angular_acceleration;

		// integrate angular velocity to get angular orientation
		//
		m_angle += dt * m_angularvelocity;

		m_zobObject->SetWorldPosition(worldPos.x, m_lastGroundPosition.y + m_heightAboveGround, worldPos.z);
		

		//sn = sinf(m_angle);
		//cs = cosf(m_angle);fr
		//ZobVector3 forward = m_zobObject->GetForward();
		//ZobVector3 up = m_zobObject->GetUp();
		//ZobVector3 left = m_zobObject->GetLeft();

		DirectZob::GetInstance()->GetEngine()->QueueLine(DirectZob::GetInstance()->GetCameraManager()->GetCurrentCamera(), &worldPos, &dp, 0xFF00FF, true, true);

		if (dp.length2() != 0)
		{
			ZobVector3 forward = ZobVector3(sn, 0, cs);
			//forward.Mul(-1);
			forward.Normalize();
			ZobVector3 left = ZobVector3::Cross(&forward, &ZobVector3::Vector3Y);
			left.Normalize();
			left.Mul(-1.0f);
			ZobVector3 up = ZobVector3::Cross(&forward, &left);
			up.Normalize();
			//left.Mul(-1.0f);
			m_zobObject->LookAt(&forward, &left, &up, false);
		}
		

		

	}
	Text2D* m_textManager = directZob->GetTextManager();
	ZobVector3 c = ZobVector3(0.0f, 0.0f, 0.0f);
	DirectZob::GetInstance()->GetHudManager()->Print(0.1f, 0.9f, 0.025f, 0.025f, &c, "ST %.2f A %.2f AV %.2f\n V %.2f, %.2f, %.2f\n A %.2f, %.2f, %.2f", m_steerangle, m_angle, m_angularvelocity, m_velocityWorld.x, m_velocityWorld.y, m_velocityWorld.z, m_accelerationWorld.x, m_accelerationWorld.y, m_accelerationWorld.z);

}

void ZobBehaviorCar::Update_old(float dt)
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