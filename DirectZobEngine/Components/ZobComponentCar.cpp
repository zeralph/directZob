#include "ZobComponentCar.h"
#include "../DirectZob.h"
#include "../Rendering/Engine.h"
#include "../ZobPhysic/ZobPhysicsEngine.h"
#include "../ZobPhysic/ZobPhysicComponent.h"
#include "../Managers/ZobInputManager.h"
#include "../Rendering/text2D.h"
#include "../ZobObjects/ZobObject.h"
#include "../Managers/CameraManager.h"
#include "ZobComponentFactory.h"
#include <math.h>

#define MS_TO_KMH(a) a * 3.6f 
#define KMH_TO_MS(a) a / 3.6f 

#undef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)


ZobComponentCar::~ZobComponentCar()
{

}

ZobComponentCar::ZobComponentCar(ZobObject* zobObject) : ZobComponent(zobObject, false)
{
	SET_CLASS_AND_NAME
	m_carType = eCarType_pouet;
	m_componentType = ZobComponentFactory::eComponent_car;
	m_lastGroundPosition = ZobVector3(0, 0, 0);
	m_lastGroundNormal = ZobVector3(0, 1, 0);
	m_frontLeftWheel = NULL;
	m_frontRightWheel = NULL;
	m_rearLeftWheel = NULL;
	m_rearRightWheel = NULL;
	m_lastCollLocalContact = ZobVector3(0, 0, 0);
	m_lastCollPosition = ZobVector3(0, 0, 0);
	m_lastCollDirection= ZobVector3(0, 0, 0);
	m_lastCollRebound  = ZobVector3(0, 0, 0);
	m_lastCollNormal   = ZobVector3(0, 0, 0);

	m_wheelRotationAngle = 0;
	Init(NULL);

	m_test = NULL;
	m_varExposer->WrapEntity<ZobObject*>("Front left wheel", &m_frontLeftWheel, NULL, false, true);
	m_varExposer->WrapEntity<ZobObject*>("Front right wheel", &m_frontRightWheel, NULL, false, true);
	m_varExposer->WrapEntity<ZobObject*>("Rear left wheel", &m_rearLeftWheel, NULL, false, true);
	m_varExposer->WrapEntity<ZobObject*>("Rear right wheel", &m_rearRightWheel, NULL, false, true);
	m_varExposer->WrapEntity<ZobComponentSprite*>("test", &m_test, NULL, false, true);
	m_varExposer->WrapVariable<float>("Speed", &m_speed_ms, NULL, true, false);
	m_varExposer->WrapVariable<bool>("HandBrake", &m_handBrake, NULL, true, true);
	m_varExposer->WrapVariable<float>("Mass", &m_mass, NULL, false, true);
	m_varExposer->WrapVariable<float>("Max friction", &m_maxGrip, NULL, false, true);
	m_varExposer->WrapVariable<float>("Inertia", &m_inertia, NULL, false, true);
	m_varExposer->WrapVariable<float>("Air resistance", &m_drag, NULL, false, true);
	m_varExposer->WrapVariable<float>("Rolling resistance", &m_resistance, NULL, false, true);
	m_varExposer->WrapVariable<float>("Front cornering stiffness", &m_ca_f, NULL, false, true);
	m_varExposer->WrapVariable<float>("Rear cornering stiffness", &m_ca_r, NULL, false, true);
	m_varExposer->WrapVariable<float>("Steering rotation speed", &m_steeringRotationSpeedRS, NULL, false, true);

	//m_zobObject->GetWorldRotation().y;
	m_speed_ms = 0;
	m_angle = M_PI / 2.0;
	ZobVector3 startRotation = m_zobObject->GetWorldRotation();
	m_angle -= DEG_TO_RAD(startRotation.y);
	m_angularvelocity = 0;
	m_steerangle = 0;
	m_throttle = 0;
	m_brake = 0;
	m_mass = 1500;
	m_maxGrip = 10.0f;
	m_inertia = 1500;
	m_drag = 5.0;
	m_resistance = 30.0;
	m_ca_r = -5.20;
	m_ca_f = -5.0;
	m_heightAboveGround = 0.9f;
	m_handBrake = false;
	m_velocityWorld = ZobVector3(0, 0, 0);
	m_accelerationWorld = ZobVector3(0, 0, 0);
	m_hadCollision = false;
	m_goingRear = false;
	m_maxSteeringAngle = M_PI / 4.0f;
	m_steeringRotationSpeedRS = M_PI;
}

void ZobComponentCar::PreUpdate(float dt)
{
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		CheckGroundCollisions();
		CheckEnvironmentCollision();
		UpdateInputs(dt);
		double	rot_angle = 0.0;
		double	sideslip = 0.0;
		double	slipanglefront = 0.0;
		double	slipanglerear = 0.0;
		double	torque = 0.0;
		double	yawspeed = 0.0;
		double	weight = 0.0;
		float sn = 0.0f;
		float cs = 0.0f;
		double	angular_acceleration = 0.0;
		ZobVector3 flatf = ZobVector3(0, 0, 0);
		ZobVector3 flatr = ZobVector3(0, 0, 0);
		int		front_slip = 0;
		float 	motorBreak = 0.0f;
		ZobVector3 resistance = ZobVector3(0, 0, 0);
		ZobVector3 ftraction = ZobVector3(0, 0, 0);
		ZobVector3 force = ZobVector3(0, 0, 0);
		ZobVector3 acceleration = ZobVector3(0, 0, 0);
		sn = sin(m_angle);
		cs = cos(m_angle);
		float velocityX = 0.f;
		float velocityY = 0.f;
		float velocityZ = 0.f;
		if (m_steerangle != 0.0f)
		{
			int breakme = 1;
		}
		assert(!m_velocityWorld.isNaN());
		ZobVector3 worldPos = m_zobObject->GetWorldPosition();
		m_speed_ms = m_velocityWorld.sqrtLength();
		// SAE convention: x is to the front of the car, y is to the right, z is down

		//	bangz: Velocity of Car. Vlat and Vlong
		// transform velocity in world reference frame to velocity in car reference frame
		velocityZ = cs * m_velocityWorld.x + sn * m_velocityWorld.z;
		velocityY = m_velocityWorld.y;
		velocityX = -sn * m_velocityWorld.x + cs * m_velocityWorld.z;
		if (m_speed_ms < 1)
		{
			//m_steerangle = 0;
		}

		// longtitudinal force on rear wheels - very simple traction model
		if (m_throttle == 0)
		{
			//			motorBreak = 10.0f; //motor break;
		}
		ftraction.z = 150 * (m_throttle - (m_brake + motorBreak));// *SGN(velocity.z));
		ftraction.x = 0;
		ftraction.y = 0;

		if (m_handBrake)
			ftraction.z *= 0.5;

		if (velocityZ < 0)
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
		if (velocityZ == 0)		// TODO: fix singularity
			rot_angle = 0;
		else
			rot_angle = atan2(yawspeed, fabsf(velocityZ));

		// Calculate the side slip angle of the car (a.k.a. beta)
		if (velocityX == 0)		// TODO: fix singularity
			sideslip = 0;
		else
			sideslip = atan2(velocityX, fabsf(velocityZ));

		assert(!isnan(rot_angle));
		assert(!isnan(sideslip));
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
		resistance.x = -(m_resistance * velocityX + m_drag * velocityX * abs(velocityX));
		resistance.y = 0;//-(RESISTANCE * velocity.y + DRAG * velocity.y * abs(velocity.y));
		resistance.z = -(m_resistance * velocityZ + m_drag * velocityZ * abs(velocityZ));

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

		if (m_hadCollision)
		{
			/*
			ZobVector3 r = m_zobObject->GetWorldPosition() - m_lastCollPosition;
			r.Normalize();
			r.z = cs * r.x + sn * r.z;
			r.y = r.y;
			r.x = -sn * r.x + cs * r.z;
			float l = m_velocityWorld.sqrtLength();
			velocity.x = r.x * (2.0f);
			//velocity.y = 0;
			//velocity.z = abs(r.z) * SGN(velocity.z) * (l * 0.7f);
			*/
			m_accelerationWorld.x = 0;
			m_accelerationWorld.y = 0;
			m_accelerationWorld.z = 0;
			float f = SGN(m_lastCollLocalContact.x) * 10.5f;// (m_speed_ms * 0.7f + 1.0f);
			velocityX = f;
			velocityZ *= 0.7f;
			m_velocityWorld.z = cs * velocityX + sn * velocityZ;
			m_velocityWorld.y = 0;
			m_velocityWorld.x = -sn * velocityX + cs * velocityZ;
			m_hadCollision = false;
		}
		else
		{
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

			if (m_speed_ms < 1)
			{
				if (m_throttle == 0 && m_brake == 0)
				{
					m_velocityWorld.x = 0;
					m_velocityWorld.z = 0;
				}
				angular_acceleration = 0;
				m_angularvelocity = 0;
			}
		}
		// position is integrated velocity
		//
		worldPos.x += dt * m_velocityWorld.x;
		worldPos.z += dt * m_velocityWorld.z;
		m_goingRear = velocityZ < 0;

		// Angular velocity and heading

		// integrate angular acceleration to get angular velocity
		//
		m_angularvelocity += dt * angular_acceleration;

		// integrate angular velocity to get angular orientation
		//
		m_angle += dt * m_angularvelocity;
		assert(!isnan(worldPos.x));
		m_zobObject->SetWorldPosition(worldPos.x, m_lastGroundPosition.y + m_heightAboveGround, worldPos.z);
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
		UpdateWheels(dt);
	}
	ZobHUDManager* h = DirectZob::GetInstance()->GetHudManager();
	//h->Print(0.1f, 0.9f, 0.025f, 0.025f, &c, "ST %.2f A %.2f AV %.2f\n V %.2f, %.2f, %.2f\n A %.2f, %.2f, %.2f", m_steerangle, m_angle, m_angularvelocity, m_velocityWorld.x, m_velocityWorld.y, m_velocityWorld.z, m_accelerationWorld.x, m_accelerationWorld.y, m_accelerationWorld.z);
	float kmh = MS_TO_KMH(m_speed_ms);
	//h->Print(ZobHUDManager::eHudUnit_ratio, 0.1f, 0.8f, 1, "Leelawadee UI", &ZobColor::Red, "ANGLE %i", (int)RAD_TO_DEG(m_angle));
	//h->Print(ZobHUDManager::eHudUnit_ratio, 0.8f, 0.9f, 1, "Leelawadee UI", &ZobColor::Red, "%.0f Kmh", kmh);
	//h->Print(ZobHUDManager::eHudUnit_ratio, 0.1f, 0.9f, 1, "Leelawadee UI", &ZobColor::Red, "wheels %.2f", m_steerangle);
}

void ZobComponentCar::Init(DirectZobType::sceneLoadingCallback cb)
{
	if (cb)
	{
		cb(0, 0, m_name);
	}
	ReLoadVariables();
}

void ZobComponentCar::CheckEnvironmentCollision()
{
	ZobPhysicComponent* zc = m_zobObject->m_physicComponent;
	ZobPhysicComponent::collision* coll = zc->GetLastCollision();
	if (!coll->handled)
	{
		//if (coll->contactType == ZobPhysicComponent::eContactType::ContactStart)
		{
			if ((coll->collisionLayer & ZobPhysicComponent::eLayer_wall))
			{
				m_lastCollLocalContact = coll->collisionLocalContactPoint;
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
		//else if (coll->contactType == ZobPhysicComponent::eContactType::ContactExit)
		//{
		//	m_hadCollision = false;
		//}
		coll->Reset();
	}
}

reactphysics3d::decimal ZobComponentCar::GroundRaycastClass::notifyRaycastHit(const reactphysics3d::RaycastInfo& info) 
{
	if (Component)
	{
		Component->m_lastGroundPosition.x = info.worldPoint.x;
		Component->m_lastGroundPosition.y = info.worldPoint.y;
		Component->m_lastGroundPosition.z = info.worldPoint.z;
		float r = 0.9f;
		Component->m_lastGroundNormal.x = Component->m_lastGroundNormal.x * r + (1.0f - r) * info.worldNormal.x;
		Component->m_lastGroundNormal.y = Component->m_lastGroundNormal.y * r + (1.0f - r) * info.worldNormal.y;
		Component->m_lastGroundNormal.z = Component->m_lastGroundNormal.z * r + (1.0f - r) * info.worldNormal.z;
	}
	return reactphysics3d::decimal(1.0);
}

void ZobComponentCar::UpdateWheels(float dt)
{
	if (m_rearLeftWheel)
	{
		Quaternion q = Quaternion::identity();
		float wheelRadius = 0.3f;
		float f = dt * m_speed_ms / (2.0f * M_PI * wheelRadius);
		m_wheelRotationAngle += f * 360.0f;
		f = DEG_TO_RAD(m_wheelRotationAngle);
		if (m_goingRear)
		{
			f = -f;
		}
		float g = m_steerangle;
		Vector3 v = Vector3(-f, 0, 0);
		q = q.fromEulerAngles(v);
		q.normalize();
		m_rearLeftWheel->m_physicComponent->SetLocalOrientation(q, false);
		m_rearRightWheel->m_physicComponent->SetLocalOrientation(q, false);
		v = Vector3(-f, -g, 0);
		q = q.fromEulerAngles(v);
		q.normalize();
		m_frontLeftWheel->m_physicComponent->SetLocalOrientation(q, false);
		m_frontRightWheel->m_physicComponent->SetLocalOrientation(q, false);
	}
}

void ZobComponentCar::CheckGroundCollisions()
{
	DirectZob* directZob = DirectZob::GetInstance();
	RigidBody* rb = m_zobObject->m_physicComponent->GetRigidBody();
	Vector3 p = Vector3(rb->getTransform().getPosition());
	Vector3 e = Vector3(rb->getTransform().getPosition());
	p.y += 1000.0f;
	e.y -= 1000.0f;
	reactphysics3d::Ray ray(p, e);
	reactphysics3d::RaycastInfo info;
	ZobComponentCar::GroundRaycastClass cb;
	cb.Component = this;
	short categ = ZobPhysicComponent::eLayer_ground;
	DirectZob::GetInstance()->GetPhysicsEngine()->GetWorld()->raycast(ray, &cb, categ);
}

void ZobComponentCar::UpdateInputs(float dt)
{
	const gainput::InputMap* inputMap = DirectZob::GetInstance()->GetInputManager()->GetMap();
	float inputAcc = 0.0f;
	float inputBrk = 0.0f;
	float inputDir = 0.0f;
	inputAcc = inputMap->GetFloat(ZobInputManager::RightShoulder);
	inputBrk = inputMap->GetFloat(ZobInputManager::LeftShoulder);
	inputDir = inputMap->GetFloat(ZobInputManager::LeftStickX);
	float wantedSteerAngle = m_maxSteeringAngle * inputDir;
	float deltaAngle = m_steerangle - wantedSteerAngle;
	float rotSpeed = m_steeringRotationSpeedRS;
	if (wantedSteerAngle == 0)
	{
		rotSpeed = m_steeringRotationSpeedRS * 2.0f;
	}
	m_steerangle = m_steerangle - (deltaAngle * dt * rotSpeed);
	m_steerangle = clamp(m_steerangle, -m_maxSteeringAngle, m_maxSteeringAngle);
	m_throttle = 100.0f * inputAcc;
	m_brake = 100.0f * inputBrk;
	m_handBrake = inputMap->GetBool(ZobInputManager::Handbrake);
	//update current camera
	if (inputMap->GetBoolIsNew(ZobInputManager::NextCamera))
	{
		DirectZob::GetInstance()->GetCameraManager()->SwitchToNextAvailableCamera();
	}
}

void ZobComponentCar::EditorUpdate()
{
}

void ZobComponentCar::DrawGizmos(const Camera* camera, const ZobVector3* position, const ZobVector3* rotation) const
{
	//return;
	DirectZob* directZob = DirectZob::GetInstance();
	if (directZob->IsPhysicPlaying())
	{
		Engine* e = DirectZob::GetInstance()->GetEngine();
		ZobVector3 p = m_zobObject->GetWorldPosition();
		ZobVector3 v = m_lastCollPosition;
		
		e->QueueLine(camera, &p, &v, &ZobColor::Yellow, true, true);

		ZobMatrix4x4 m = ZobMatrix4x4();
		m.AddTranslation(m_lastCollPosition);
		e->QueueSphere(camera, &m, 0.1f, &ZobColor::Magenta, false, false);

	}
}