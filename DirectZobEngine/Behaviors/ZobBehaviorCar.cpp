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
#define MAX_GRIP	8.0				/* maximum (normalised) friction force, =diameter of friction circle */


ZobBehaviorCar::~ZobBehaviorCar()
{

}

ZobBehaviorCar::ZobBehaviorCar(ZobObject* zobObject, TiXmlElement* node) : ZobBehavior(zobObject, node)
{
	m_type = eBehavior_car;
	m_lastGroundPosition = ZobVector3(0, 0, 0);
	m_lastGroundNormal = ZobVector3(0, 1, 0);

	m_lastCollPosition = ZobVector3(0, 0, 0);
	m_lastCollDirection= ZobVector3(0, 0, 0);
	m_lastCollRebound  = ZobVector3(0, 0, 0);
	m_lastCollNormal   = ZobVector3(0, 0, 0);
	Init();
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Init()
{
	//m_zobObject->GetWorldRotation().y;
	m_speed_ms = 0;
	m_angle = M_PI / 2.0;
	float a = DEG_TO_RAD(m_zobObject->GetWorldRotation().y);
	m_angle -= a;
	m_angularvelocity=0;
	m_steerangle=0;
	m_throttle=0;
	m_brake=0;
	m_mass = 1500;
	m_inertia = 1500;
	m_heightAboveGround = 0.9f;
	m_handBrake = false;
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
	m_steerangle = (M_PI / 4.0) * inputDir;
	m_throttle = 100.0f * inputAcc;
	m_brake = 100.0f * inputBrk;
	m_handBrake = inputMap->GetBool(ZobInputManager::Handbrake);
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
		int		front_slip=0;
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
		m_speed_ms = m_velocityWorld.sqrtLength();
		// SAE convention: x is to the front of the car, y is to the right, z is down

		//	bangz: Velocity of Car. Vlat and Vlong
		// transform velocity in world reference frame to velocity in car reference frame
		velocity.z = cs* m_velocityWorld.x + sn * m_velocityWorld.z;
		velocity.y = m_velocityWorld.y;
		velocity.x = -sn * m_velocityWorld.x + cs * m_velocityWorld.z;

		if (m_speed_ms < 5)
		{
			velocity.z = 0;
			velocity.x = 0;
		}

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
		if (m_handBrake)
			flatr.x *= 0.5;

		// longtitudinal force on rear wheels - very simple traction model
		ftraction.z = 100 * (m_throttle - m_brake * SGN(velocity.z));
		ftraction.x = 0;
		ftraction.y = 0;
		if (m_handBrake)
			ftraction.z *= 0.5;

		// Forces and torque on body

		// drag and rolling resistance
		resistance.x = -(RESISTANCE * velocity.x + DRAG * velocity.x * abs(velocity.x));
		resistance.y = -(RESISTANCE * velocity.y + DRAG * velocity.y * abs(velocity.y));
		resistance.z = -(RESISTANCE * velocity.z + DRAG * velocity.z * abs(velocity.z));

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
		m_accelerationWorld.z = cs * acceleration.x + sn * acceleration.z;
		m_accelerationWorld.y = 0;
		m_accelerationWorld.x = -sn * acceleration.x + cs * acceleration.z;
		
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
		dp.z = worldPos.z - dp.z;

		// Angular velocity and heading

		// integrate angular acceleration to get angular velocity
		//
		m_angularvelocity += dt * angular_acceleration;

		// integrate angular velocity to get angular orientation
		//
		m_angle += dt * m_angularvelocity;

		m_zobObject->SetWorldPosition(worldPos.x, m_lastGroundPosition.y + m_heightAboveGround, worldPos.z);

		if (dp.length2() != 0)
		{
			ZobVector3 up = ZobVector3::Vector3Y;
			ZobVector3 forward = ZobVector3(cs, 0, sn);
			//forward.Mul(-1);
			forward.Normalize();
			ZobVector3 left = ZobVector3::Cross(&forward, &up);
			left.Normalize();
			left.Mul(-1.0f);
			up = ZobVector3::Cross(&forward, &left);
			up.Normalize();
			//up = ZobVector3::Vector3Y;
			//left.Mul(-1.0f);
			m_zobObject->LookAt(&forward, &left, &up, false);
		}
	}
	ZobHUDManager* h = DirectZob::GetInstance()->GetHudManager();
	ZobVector3 c = ZobVector3(0.0f, 0.0f, 0.0f);
	//h->Print(0.1f, 0.9f, 0.025f, 0.025f, &c, "ST %.2f A %.2f AV %.2f\n V %.2f, %.2f, %.2f\n A %.2f, %.2f, %.2f", m_steerangle, m_angle, m_angularvelocity, m_velocityWorld.x, m_velocityWorld.y, m_velocityWorld.z, m_accelerationWorld.x, m_accelerationWorld.y, m_accelerationWorld.z);
	float kmh = MS_TO_KMH(m_speed_ms);
	h->Print(0.8f, 0.9f, 0.030f, 0.030f, &c, "%.0f Kmh", kmh);
	

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
		ZobVector3 v = m_zobObject->GetForward();
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