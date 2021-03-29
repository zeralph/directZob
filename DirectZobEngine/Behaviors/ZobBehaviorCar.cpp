#include "ZobBehaviorCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"
#include "../ZobObjects/ZobObject.h"

#define MS_TO_KMH(a) a * 3.6f 
#define KMH_TO_MS(a) a / 3.6f 

#undef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)


ZobBehaviorCar::~ZobBehaviorCar()
{

}

ZobBehaviorCar::ZobBehaviorCar(ZobObject* zobObject) : ZobBehavior(zobObject)
{
	m_carType = eCarType_pouet;
	m_type = eBehavior_car;
	m_lastGroundPosition = ZobVector3(0, 0, 0);
	m_lastGroundNormal = ZobVector3(0, 1, 0);

	m_lastCollPosition = ZobVector3(0, 0, 0);
	m_lastCollDirection= ZobVector3(0, 0, 0);
	m_lastCollRebound  = ZobVector3(0, 0, 0);
	m_lastCollNormal   = ZobVector3(0, 0, 0);

	int p[] = { eCarType_prout , eCarType_pouet, eCarType_truc};
	const char* s[] = { "prout", "pouet", "truc" };
	WrapEnum("carType", &m_carType, 3, p, s, false, true);

	WrapVariable(eWrapperType_bool, "HandBrake", &m_handBrake, false, true);
	WrapVariable(eWrapperType_float, "Mass", &m_mass, false, true);
	WrapVariable(eWrapperType_float, "Max friction", &m_maxGrip, false, true);
	WrapVariable(eWrapperType_float, "Inertia", &m_inertia, false, true);
	WrapVariable(eWrapperType_float, "Air resistance", &m_drag, false, true);
	WrapVariable(eWrapperType_float, "Rolling resistance", &m_resistance, false, true);
	WrapVariable(eWrapperType_float, "Front cornering stiffness", &m_ca_f, false, true);
	WrapVariable(eWrapperType_float, "Rear cornering stiffness", &m_ca_r, false, true);

	m_mass = 1500;
	m_maxGrip = 10.0f;
	m_inertia = 1500;
	m_drag = 5.0;
	m_resistance = 30.0;
	m_ca_r = -5.20;
	m_ca_f = -5.0;
}

void ZobBehaviorCar::PreUpdate()
{

}

void ZobBehaviorCar::Init()
{
	//m_zobObject->GetWorldRotation().y;
	m_speed_ms = 0;
	m_angle = M_PI / 2.0;
	ZobVector3 startRotation = m_zobObject->GetWorldRotation();
	m_angle -= DEG_TO_RAD(startRotation.y);
	m_angularvelocity=0;
	m_steerangle=0;
	m_throttle=0;
	m_brake=0;
	m_heightAboveGround = 0.9f;
	m_handBrake = false;
	m_velocityWorld = ZobVector3(0, 0, 0);
	m_hadCollision = false;
}

void ZobBehaviorCar::CheckEnvironmentCollision()
{
	ZobPhysicComponent* zc = m_zobObject->m_physicComponent;
	ZobPhysicComponent::collision* coll = zc->GetLastCollision();
	if (!coll->handled)
	{
		if (coll->contactType == ZobPhysicComponent::eContactType::ContactStart)
		{
			if ((coll->collisionLayer & ZobPhysicComponent::eLayer_wall))
			{
				m_lastCollPosition = coll->collisionWorldPosition;
				m_lastCollDirection = coll->collisionWorldDirection;
				m_lastCollNormal = coll->collisionWorldNormal;
				m_lastCollNormal.y = 0;
				m_lastCollNormal.Normalize();
				m_hadCollision = true;
				m_lastCollRebound = m_zobObject->GetWorldPosition() - m_lastCollPosition;
				m_lastCollRebound.y = 0;
				m_lastCollRebound.Normalize();
				ZobVector3 z = ZobVector3::Cross(&m_lastCollRebound, &m_lastCollNormal);
				z.Normalize();
				ZobVector3 q = ZobVector3::Cross(&m_lastCollRebound, &z);
				m_lastCollRebound = q;
				m_lastCollRebound.Normalize();
				//m_lastCollRebound.Mul(coll->penetration);
			}
			else if ((coll->collisionLayer & ZobPhysicComponent::eLayer_objects))
			{
				//other cars
			}
		}
		else if (coll->contactType == ZobPhysicComponent::eContactType::ContactExit)
		{
			m_hadCollision = false;
		}
		coll->Reset();
	}
}

reactphysics3d::decimal ZobBehaviorCar::GroundRaycastClass::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) 
{
	if (behavior)
	{
		behavior->m_lastGroundPosition.x = info.worldPoint.x;
		behavior->m_lastGroundPosition.y = info.worldPoint.y;
		behavior->m_lastGroundPosition.z = info.worldPoint.z;
		float r = 0.9f;
		behavior->m_lastGroundNormal.x = behavior->m_lastGroundNormal.x * r + (1.0f - r) * info.worldNormal.x;
		behavior->m_lastGroundNormal.y = behavior->m_lastGroundNormal.y * r + (1.0f - r) * info.worldNormal.y;
		behavior->m_lastGroundNormal.z = behavior->m_lastGroundNormal.z * r + (1.0f - r) * info.worldNormal.z;
	}
	return reactphysics3d::decimal(1.0);
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
	ZobBehaviorCar::GroundRaycastClass cb;
	cb.behavior = this;
	short categ = ZobPhysicComponent::eLayer_ground;
	DirectZob::GetInstance()->GetPhysicsEngine()->GetWorld()->raycast(ray, &cb, categ);
}

void ZobBehaviorCar::UpdateInputs(float dt)
{
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	float inputAcc = 0.0f;
	float inputBrk = 0.0f;
	float inputDir = 0.0f;
	inputAcc = inputMap->GetFloat(ZobInputManager::RightShoulder);
	inputBrk = inputMap->GetFloat(ZobInputManager::LeftShoulder);
	inputDir = inputMap->GetFloat(ZobInputManager::LeftStickX);
	float wantedSteerAngle = (M_PI / 4.0) * inputDir;
	m_steerangle = m_steerangle * dt + wantedSteerAngle * (1.0f - dt);
	m_throttle = 100.0f * inputAcc;
	m_brake = 100.0f * inputBrk;
	m_handBrake = inputMap->GetBool(ZobInputManager::Handbrake);
	//update current camera
	if (inputMap->GetBoolIsNew(ZobInputManager::NextCamera))
	{
		DirectZob::GetInstance()->GetCameraManager()->SwitchToNextAvailableCamera();
	}

	//TODO : move me elsewhere
	if (inputMap->GetBoolIsNew(ZobInputManager::Start))
	{
		std::string sceneName = "menu.dzs";
		std::string scenePath = DirectZob::GetInstance()->GetResourcePath();
		DirectZob::GetInstance()->LoadScene(scenePath, sceneName, NULL);
		return;
	}
}

void ZobBehaviorCar::EditorUpdate()
{
}

void ZobBehaviorCar::Update(float dt)
{
	CheckGroundCollisions();
	CheckEnvironmentCollision();
	UpdateInputs(dt);
	DirectZob* directZob = DirectZob::GetInstance();
	float sn, cs;
	double	angular_acceleration;
	ZobVector3 flatf, flatr;
	if (directZob->IsPhysicPlaying())
	{
		double	rot_angle;
		double	sideslip;
		double	slipanglefront;
		double	slipanglerear;
		double	torque;
		double	yawspeed;
		double	weight;
		int		front_slip=0;
		float 	motorBreak = 0.0f;
		ZobVector3 velocity, resistance;
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

		if (m_speed_ms < 1)
		{
			//m_steerangle = 0;
		}

		// longtitudinal force on rear wheels - very simple traction model
		if (m_throttle == 0)
		{
			motorBreak = 10.0f; //motor break;
		}
		ftraction.z = 150 * (m_throttle - (m_brake + motorBreak));// *SGN(velocity.z));
		ftraction.x = 0;
		ftraction.y = 0;
		if (m_handBrake)
			ftraction.z *= 0.5;

		if (velocity.z < 0)
		{
			m_steerangle = -m_steerangle;
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
			rot_angle = atan2(yawspeed, fabsf(velocity.z));

		// Calculate the side slip angle of the car (a.k.a. beta)
		if (velocity.x == 0)		// TODO: fix singularity
			sideslip = 0;
		else
			sideslip = atan2(velocity.x, fabsf(velocity.z));

		// Calculate slip angles for front and rear wheels (a.k.a. alpha)
		slipanglefront = sideslip + rot_angle - m_steerangle;
		slipanglerear = sideslip - rot_angle;

		// weight per axle = half car mass times 1G (=9.8m/s^2) 
		weight = m_mass * 9.8 * 0.5;

		// lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
		flatf.y = 0;
		flatf.z = 0;
		flatf.x = m_ca_f * slipanglefront;
		flatf.x = fmin(m_maxGrip, flatf.x);
		flatf.x = fmax(-m_maxGrip, flatf.x);
		flatf.x *= weight;
		if (front_slip)
			flatf.x *= 0.5;

		// lateral force on rear wheels
		flatr.y = 0;
		flatr.z = 0;
		flatr.x = m_ca_r * slipanglerear;
		flatr.x = fmin(m_maxGrip, flatr.x);
		flatr.x = fmax(-m_maxGrip, flatr.x);
		flatr.x *= weight;
		if (m_handBrake)
			flatr.x *= 0.5;

		// Forces and torque on body

		// drag and rolling resistance
		resistance.x = -(m_resistance * velocity.x + m_drag* velocity.x * abs(velocity.x));
		resistance.y = 0;//-(RESISTANCE * velocity.y + DRAG * velocity.y * abs(velocity.y));
		resistance.z = -(m_resistance * velocity.z + m_drag* velocity.z * abs(velocity.z));

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

		//collision
		if (m_hadCollision)
		{
			ZobVector3 z = m_velocityWorld;
			z.Normalize();
			float f = 1.0f - fabs(ZobVector3::Dot(&z, &m_lastCollRebound));
			z = ZobVector3::Cross(&z, &m_lastCollRebound);
			m_velocityWorld.z = m_lastCollRebound.z * clamp(m_speed_ms, 2.0f, 100.0f);
			m_velocityWorld.x = m_lastCollRebound.x * clamp(m_speed_ms, 2.0f, 100.0f);
			m_accelerationWorld.x = 0;
			m_accelerationWorld.y = 0;
			m_accelerationWorld.z = 0;
			m_velocityWorld.Mul(1.0f - f);
			angular_acceleration = -SGN(z.y) * f * 10.0f *clamp(m_speed_ms, 1.0f, 40.0f);
		}

		if(m_speed_ms < 1)
		{
			if(m_throttle == 0 && m_brake == 0)
			{
				m_velocityWorld.x = 0;
				m_velocityWorld.z = 0;
			}
			angular_acceleration = 0;
			m_angularvelocity = 0;
		}
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
		//if (dp.length2() != 0)
		{
			sn = sin(m_angle);
			cs = cos(m_angle);
			ZobVector3 up = ZobVector3::Vector3Y;
			up = m_lastGroundNormal;// ZobVector3::Vector3Y;
			up.Normalize();
			ZobVector3 forward = ZobVector3(cs, 0, sn);
			forward.Normalize();
			ZobVector3 left = ZobVector3::Cross(&up, &forward);
			left.Normalize();
			forward = ZobVector3::Cross(&left, &up);
			m_zobObject->LookAt(&forward, &left, &up, false);
		}
	}
	ZobHUDManager* h = DirectZob::GetInstance()->GetHudManager();
	ZobVector3 c = ZobVector3(1.0f, 0.0f, 0.0f);
	//h->Print(0.1f, 0.9f, 0.025f, 0.025f, &c, "ST %.2f A %.2f AV %.2f\n V %.2f, %.2f, %.2f\n A %.2f, %.2f, %.2f", m_steerangle, m_angle, m_angularvelocity, m_velocityWorld.x, m_velocityWorld.y, m_velocityWorld.z, m_accelerationWorld.x, m_accelerationWorld.y, m_accelerationWorld.z);
	float kmh = MS_TO_KMH(m_speed_ms);
	h->Print(ZobHUDManager::eHudUnit_ratio, 0.1f, 0.8f, 1, "Leelawadee UI", &c, "MASS : %.2f GRIP %.2f", m_mass, m_maxGrip);
	h->Print(ZobHUDManager::eHudUnit_ratio, 0.8f, 0.9f, 1, "Leelawadee UI", &c, "%.0f Kmh", kmh);
	h->Print(ZobHUDManager::eHudUnit_ratio, 0.1f, 0.9f, 1, "Leelawadee UI", &c, "wheels %.2f", m_steerangle);
}

void ZobBehaviorCar::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{
	//return;
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		Engine* e = DirectZob::GetInstance()->GetEngine();
		ZobVector3 p = m_zobObject->GetWorldPosition();
		ZobVector3 v = m_lastGroundNormal;
		
		p = m_lastCollPosition;
		v = m_lastCollDirection;
		v = v + p;
		e->QueueLine(camera, &p, &v, 0x00FFFF, true, true);
		ZobMatrix4x4 m = ZobMatrix4x4();
		m.AddTranslation(m_lastCollPosition);
		e->QueueSphere(camera, &m, 0.1f, 0xFF0000, false, false);
		p = m_lastCollPosition;
		v = m_lastCollNormal;
		v = v + p;
		e->QueueLine(camera, &p, &v, 0xFFFF00, true, true);

		p = m_lastCollPosition;
		v = m_lastCollRebound;

		v = v + p;
		e->QueueLine(camera, &p, &v, 0xFFFFFF, true, true);
	}
}