#include "Camera.h"
#include "DirectZob.h"
#include <math.h>
#include <mutex>
#include "DirectZob.h"
#include "ZobPhysic/ZobPhysicComponent.h"
#include "ZobCameraController/ZobCameraController.h"
#include "ZobCameraController/ZobCameraControllerOrbital.h"
#include "ZobCameraController/ZobCameraControllerFPS.h"
#include "ZobCameraController/ZobCameraControllerFollowCar.h"
#include "Misc/ZobGeometryHelper.h"
#include "Misc/ZobXmlHelper.h"
#include "Components/ZobComponentSprite.h"
#include "SceneLoader.h"

static std::mutex g_update_camera_mutex;
static float ee = 0.0f;

static ZobVector3 sRayDbg;
static ZobVector3 sRayDbg2;

Camera::Camera(ZobEntity::ZobType zobType, const std::string& name, eCameraType type, float fov, BufferData* bufferData, ZobObject* parent)
	:ZobObject(zobType, ZobEntity::ZobSubType::subtype_zobCamera, name, parent)
{
	m_zobCameraController = NULL;
	ChangeCameraController(type, m_type == ZobEntity::ZobType::type_editor);
	m_controlerType = type;
	m_nextControlerType = type;
	m_fov = fov;
	m_active = false;

	sRayDbg = ZobVector3(1000, 1000, 1000);
	sRayDbg2 = ZobVector3(1000, 1000, 1000);
	WrapVariables();
	if (DirectZob::IsEditorMode())
	{
		ZobComponentSprite* b = (ZobComponentSprite*)ZobComponentFactory::CreateComponent(this, "Sprite", true);
		ZobFilePath zfp = ZobFilePath("camera", SceneLoader::GetResourcePath(), "camera.png", true);
		b->Set(zfp);
		b->SetForEditor();
	}
	//m_nextTranslation = m_translation;
}

Camera::Camera(zobId id, TiXmlElement* node, ZobObject* parent)
	:ZobObject(id, node, parent)
{
	m_zobCameraController = NULL;
	m_controlerType = Camera::eCamera_unset;
	m_nextControlerType = Camera::eCamera_unset;
	WrapVariables();
	m_varExposer->ReadNode(node);
	if (DirectZob::IsEditorMode())
	{
		ZobComponentSprite* b = (ZobComponentSprite*)ZobComponentFactory::CreateComponent(this, "Sprite", true);
		ZobFilePath zfp = ZobFilePath("camera", SceneLoader::GetResourcePath(), "camera.png", true);
		b->Set(zfp);
		b->SetForEditor();
	}
	//ChangeCameraController(m_nextControlerType);
}

void Camera::ChangeCameraController(eCameraType newType, bool bEditorZobComponent)
{
	if (m_zobCameraController)
	{
		delete m_zobCameraController;
		m_zobCameraController = NULL;
	}
	switch (newType)
	{
		case eCamera_fps:
		{
			m_zobCameraController = new ZobCameraControllerFPS(this, bEditorZobComponent);
			break;
		}
		case eCamera_orbital:
		{
			m_zobCameraController = new ZobCameraControllerOrbital(this, false, bEditorZobComponent);
			break;
		}
		case eCamera_orbital_free:
		{
			m_zobCameraController = new ZobCameraControllerOrbital(this, true, bEditorZobComponent);
			break;
		}
		case eCamera_base:
		{
			m_zobCameraController = new ZobCameraController(this, bEditorZobComponent);
			break;
		}
		case eCamera_followCar:
		{
			m_zobCameraController = new ZobCameraControllerFollowCar(this, bEditorZobComponent);
			break;
		}
		default:
		{
			DirectZob::LogError("Error creating camera");
			m_zobCameraController = new ZobCameraController(this, bEditorZobComponent);
			break;
		}
	}
	m_zobCameraController->Init(NULL);
}

Camera::~Camera()
{
	DirectZob::LogInfo("Delete Camera %s", m_name.c_str());
	DirectZob::AddIndent();
	DirectZob::GetInstance()->GetCameraManager()->RemoveCamera(this);
	delete m_zobCameraController;
	m_zobCameraController = NULL;
	DirectZob::RemoveIndent();
}

void Camera::WrapVariables()
{

	m_varExposer->WrapVariable<float>("FOV", &m_fov, NULL, false, true);
	eCameraType ct[7] = { eCameraType::eCamera_base,
		eCameraType::eCamera_revolving,
		eCameraType::eCamera_orbital,
		eCameraType::eCamera_fps,
		eCameraType::eCamera_follow,
		eCameraType::eCamera_followCar,
		eCameraType::eCamera_orbital_free, };
	const char* ctStr[7] = { "base", "Revolving", "Orbital", "FPS", "Follow", "FollowCar", "Orbital Free"};
	m_varExposer->WrapEnum<eCameraType>("Camera controller", &m_nextControlerType, 7, ct, ctStr, NULL, false, true);
}

void Camera::DrawGizmos(const Camera* camera, Engine* engine)
{
	if (!engine->DrawCameraGizmos())
	{
		//return;
	}
	if (ZobEntity::GetType(this->GetIdValue()) == ZobEntity::type_editor)
	{
		//		return;
	}
	if (m_zobCameraController)
	{
		m_zobCameraController->DrawGizmos(camera, engine);
	}
	ZobObject::DrawGizmos(camera, engine);
	ZobVector3 v0 = ZobVector3(-2, 1, 1);
	ZobVector3 v1 = ZobVector3(2, 1, 1);
	ZobVector3 v2 = ZobVector3(-2, -1, 1);
	ZobVector3 v3 = ZobVector3(2, -1, 1);
	m_modelMatrix.Mul(&v0);
	m_modelMatrix.Mul(&v1);
	m_modelMatrix.Mul(&v2);
	m_modelMatrix.Mul(&v3);
	ZobVector3 p = GetWorldPosition();	//loca ?
	v0 = v0;
	v1 = v1;
	v2 = v2;
	v3 = v3;
	if (this->GetName() != "EditorCamera")
	{
		engine->QueueLine(camera, &v0, &v1,&ZobColor::Black,  true, false);
		engine->QueueLine(camera, &v1, &v3,&ZobColor::Black,  true, false);
		engine->QueueLine(camera, &v2, &v3,&ZobColor::Black, true, false);
		engine->QueueLine(camera, &v2, &v0,&ZobColor::Black,  true, false);
		engine->QueueLine(camera, &p, &v1, &ZobColor::Black,   true, false);
		engine->QueueLine(camera, &p, &v2, &ZobColor::Black,   true, false);
		engine->QueueLine(camera, &p, &v3, &ZobColor::Black,   true, false);
		engine->QueueLine(camera, &p, &v0, &ZobColor::Black,   true, false);
	}
}

void Camera::Zoom(float z)
{
	m_zobCameraController->Zoom(z);
}

void Camera::RotateOrbital(ZobVector3 *center, float x, float y, float dist)
{
	float ax = x * M_PI / 180.0;
	float ay = y * M_PI / 180.0;
	Vector3 pax = Vector3(GetUp().x, GetUp().y, GetUp().z);
	pax = Vector3(0, 1, 0);
	Quaternion qx = ZobPhysicComponent::QuaternionFromAxisAngle(&pax, ax);
	qx.normalize();
	Vector3 pay = Vector3(GetLeft().x, GetLeft().y, GetLeft().z);
	pay = Vector3(1, 0, 0);
	Quaternion qy = ZobPhysicComponent::QuaternionFromAxisAngle(&pay, ay);
	Transform parentWorldT = Transform::identity();
	Transform thisWorldT = GetPhysicComponent()->GetWorldTransform();
	Vector3 vdist = Vector3(0, 0, dist);
	Quaternion q = qx * qy;
	q.normalize();
	if (q.isValid() && q.isFinite())
	{
		parentWorldT.setPosition(Vector3(center->x, center->y, center->z));
		parentWorldT.setOrientation(q);
		thisWorldT.setPosition(vdist);
		thisWorldT.setOrientation(q.getInverse());
		thisWorldT = parentWorldT * thisWorldT;
		m_physicComponent->SetWorldTransform(thisWorldT);
		LookAt(center, false);
	}
}

void Camera::RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle, bool recomputeVectors)
{
	angle = angle* M_PI / 180.0;
	Vector3 paxis = Vector3(axis->x, axis->y, axis->z);
	Vector3 p = Vector3(point->x, point->y, point->z);
	Transform parentWorldT = m_parent->GetPhysicComponent()->GetWorldTransform();
	Transform thisWorldT = GetPhysicComponent()->GetWorldTransform();
	Vector3 vdist = parentWorldT.getPosition() - thisWorldT.getPosition();
	//float dist = vdist.length();
	//vdist = Vector3(0, 0, dist);
	Quaternion q = ZobPhysicComponent::QuaternionFromAxisAngle(&paxis, angle);
	q.normalize();
	if (q.isValid() && q.isFinite())
	{
		parentWorldT.setOrientation(q);
		thisWorldT.setPosition(vdist);
		thisWorldT.setOrientation(Quaternion::identity());
		thisWorldT = parentWorldT * thisWorldT;
		m_physicComponent->SetWorldTransform(thisWorldT);
		LookAt(point, false);
	}
}

void Camera::RecomputeFLUVectors(const ZobVector3* forwardV, const ZobVector3* upV)
{
	Quaternion q = m_physicComponent->GetWorldTransform().getOrientation();
	Vector3 left = q * Vector3(1, 0, 0);
	Vector3 up = q * Vector3(0, 1, 0);
	Vector3 forward = q * Vector3(0, 0, 1);
	m_left.x = left.x;
	m_left.y = left.y;
	m_left.z = left.z;
	m_up.x = up.x;
	m_up.y = up.y;
	m_up.z = up.z;
	m_forward.x = forward.x;
	m_forward.y = forward.y;
	m_forward.z = forward.z;
	/*
	ZobVector3 fw = forwardV;
	fw.Normalize();
	ZobVector3 up = upV;
	ZobVector3 left = ZobVector3::Cross(&up, forwardV);
	left.Normalize();
	up = ZobVector3::Cross(&fw, &left);
	up.Normalize();
	m_forward = fw;
	m_up = up;
	m_left = left;
	*/
}

void Camera::Move(float dx, float dz, float dy, bool moveTargetVector)
{
	m_zobCameraController->Move(dx, dy, dz);
}

void Camera::SetTarget(const ZobVector3* t)
{
	m_zobCameraController->SetTarget(t);
}

void Camera::Init(DirectZobType::sceneLoadingCallback cb)
{
	ZobObject::Init(cb);
	ChangeCameraController(m_nextControlerType, m_type == ZobEntity::ZobType::type_editor);
	m_zobCameraController->Init(cb);
}

void Camera::PreUpdate(float dt)
{
	ZobObject::PreUpdate(dt);
	m_zobCameraController->PreUpdate(dt);
}

void Camera::Move(float x, float y, float z)
{
	m_zobCameraController->Move(x, y, z);
}

void Camera::Update(float dt)
{
	if (m_nextControlerType != m_controlerType)
	{
		ChangeCameraController(m_nextControlerType, m_type == ZobEntity::ZobType::type_editor);
		m_controlerType= m_nextControlerType;
	}
	m_zobCameraController->Update(dt);
	ZobObject::Update(dt);
	UpdateViewProjectionMatrix();
	if (!DirectZob::GetInstance()->GetEngine()->LockFrustrum())
	{
		RecomputeFrustrumPlanes();
	}
}

void Camera::PostUpdate()
{
	ZobObject::PostUpdate();
	//UpdateViewProjectionMatrix();
	//if (!DirectZob::GetInstance()->GetEngine()->LockFrustrum())
	//{
	//	RecomputeFrustrumPlanes();
	//}
}


void Camera::RecomputeFrustrumPlanes()
{
	ZobMatrix4x4 comboMatrix = ZobMatrix4x4(m_projMatrix);
	ZobMatrix4x4 transpose = m_viewRotMatrix;
	comboMatrix.Mul(&transpose);
	m_frustrumPlanes[eFrustrumPlaneLeft].a = comboMatrix.GetValue(3, 0) + comboMatrix.GetValue(0, 0);
	m_frustrumPlanes[eFrustrumPlaneLeft].b = comboMatrix.GetValue(3, 1) + comboMatrix.GetValue(0, 1);
	m_frustrumPlanes[eFrustrumPlaneLeft].c = comboMatrix.GetValue(3, 2) + comboMatrix.GetValue(0, 2);
	m_frustrumPlanes[eFrustrumPlaneLeft].d = comboMatrix.GetValue(3, 3) + comboMatrix.GetValue(0, 3);
	// Right clipping plane
	m_frustrumPlanes[eFrustrumPlaneRight].a = comboMatrix.GetValue(3, 0) - comboMatrix.GetValue(0, 0);
	m_frustrumPlanes[eFrustrumPlaneRight].b = comboMatrix.GetValue(3, 1) - comboMatrix.GetValue(0, 1);
	m_frustrumPlanes[eFrustrumPlaneRight].c = comboMatrix.GetValue(3, 2) - comboMatrix.GetValue(0, 2);
	m_frustrumPlanes[eFrustrumPlaneRight].d = comboMatrix.GetValue(3, 3) - comboMatrix.GetValue(0, 3);
	// Top clipping plane
	m_frustrumPlanes[eFrustrumPlaneNear].a = comboMatrix.GetValue(3, 0) - comboMatrix.GetValue(1, 0);
	m_frustrumPlanes[eFrustrumPlaneNear].b = comboMatrix.GetValue(3, 1) - comboMatrix.GetValue(1, 1);
	m_frustrumPlanes[eFrustrumPlaneNear].c = comboMatrix.GetValue(3, 2) - comboMatrix.GetValue(1, 2);
	m_frustrumPlanes[eFrustrumPlaneNear].d = comboMatrix.GetValue(3, 3) - comboMatrix.GetValue(1, 3);
	// Bottom clipping plane
	m_frustrumPlanes[eFrustrumPlaneFar].a = comboMatrix.GetValue(3, 0) + comboMatrix.GetValue(1, 0);
	m_frustrumPlanes[eFrustrumPlaneFar].b = comboMatrix.GetValue(3, 1) + comboMatrix.GetValue(1, 1);
	m_frustrumPlanes[eFrustrumPlaneFar].c = comboMatrix.GetValue(3, 2) + comboMatrix.GetValue(1, 2);
	m_frustrumPlanes[eFrustrumPlaneFar].d = comboMatrix.GetValue(3, 3) + comboMatrix.GetValue(1, 3);
	// Near clipping plane
	m_frustrumPlanes[eFrustrumPlaneTop].a = comboMatrix.GetValue(3, 0) + comboMatrix.GetValue(2, 0);
	m_frustrumPlanes[eFrustrumPlaneTop].b = comboMatrix.GetValue(3, 1) + comboMatrix.GetValue(2, 1);
	m_frustrumPlanes[eFrustrumPlaneTop].c = comboMatrix.GetValue(3, 2) + comboMatrix.GetValue(2, 2);
	m_frustrumPlanes[eFrustrumPlaneTop].d = comboMatrix.GetValue(3, 3) + comboMatrix.GetValue(2, 3);
	// Far clipping plane
	m_frustrumPlanes[eFrustrumPlaneBottom].a = comboMatrix.GetValue(3, 0) - comboMatrix.GetValue(2, 0);
	m_frustrumPlanes[eFrustrumPlaneBottom].b = comboMatrix.GetValue(3, 1) - comboMatrix.GetValue(2, 1);
	m_frustrumPlanes[eFrustrumPlaneBottom].c = comboMatrix.GetValue(3, 2) - comboMatrix.GetValue(2, 2);
	m_frustrumPlanes[eFrustrumPlaneBottom].d = comboMatrix.GetValue(3, 3) - comboMatrix.GetValue(2, 3);

	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneLeft]);
	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneRight]);
	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneNear]);
	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneFar]);
	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneTop]);
	NormalizePlane(m_frustrumPlanes[eFrustrumPlaneBottom]);
}

void Camera::NormalizePlane(Plane& plane)
{ 
	float   mag;    
	mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);    
	plane.a = plane.a / mag;    
	plane.b = plane.b / mag;
	plane.c = plane.c / mag;
	plane.d = plane.d / mag;
}

Camera::eFrustrumPlanes Camera::PointIsInFrustrum(const ZobVector3* pt) const
{
	ZobVector3 pv;
	Plane p;
	float d; 
	for (int i = 0; i < __eFrustrumPlane_MAX__; ++i)
	{
		p = m_frustrumPlanes[i];
		pv = ZobVector3(p.a, p.b, p.c);
		d = ZobVector3::Dot(&pv, pt) + p.d;
		if (d < 0)
		{
			return (Camera::eFrustrumPlanes)i;
		}
	}
	return __eFrustrumPlane_MAX__;
}

bool Camera::ClipSegmentToFrustrum(ZobVector3* p1, ZobVector3* p2, float& outP2Factor) const
{
	return ClipSegmentToPlanes(p1, p2, m_frustrumPlanes, outP2Factor);
}

bool Camera::ClipSegmentToPlanes(ZobVector3* p1, ZobVector3* p2, const DirectZobType::Plane* planes, float& outP2Factor)
{
	outP2Factor = 1.0f;
	ZobVector3 dp = ZobVector3(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z);
	float p1_fac = 0.0f;
	float p2_fac = 1.0f;
	for (int i = 0; i < __eFrustrumPlane_MAX__; i++)
	{
		Plane p = planes[i];
		ZobVector3 pv = ZobVector3(p.a, p.b, p.c);
		float div = ZobVector3::Dot(&pv, &dp);
		if (div != 0.0f)
		{
			float t = -(ZobVector3::Dot(&pv, p1) + p.d);
			if (div > 0.0f)
			{
				if (t >= div)
				{
					return false;
				}
				if (t > 0.0f)
				{
					float fac = t / div;
					if (fac > p1_fac)
					{
						p1_fac = fac;
						if (p1_fac > p2_fac)
						{
							return false;
						}
					}
				}
			}
			else if(div<0.0f)
			{
				if (t > 0.0f) 
				{
					return false;
				}
				if (t > div)
				{
					float fac = t / div;
					if (fac < p2_fac)
					{
						p2_fac = fac;
						if (p1_fac > p2_fac)
						{
							return false;
						}
					}
				}
			}
		}
	}
	p2_fac -= p1_fac;
	p1->x = p1->x + dp.x * p1_fac;
	p1->y = p1->y + dp.y * p1_fac;
	p1->z = p1->z + dp.z * p1_fac;
	p2->x = p1->x + dp.x * p2_fac;
	p2->y = p1->y + dp.y * p2_fac;
	p2->z = p1->z + dp.z * p2_fac;
	outP2Factor = p2_fac;
	return true;
}

bool Camera::AABBIsInFrustrum(const ZobVector3* aabbMin, const ZobVector3* aabbMax) const
{
	int INTERSECT = 2;
	int INSIDE = 1;
	int OUTSIDE = 0;
	
	int ret = INSIDE;
	ZobVector3 vmin, vmax, normal;

	for (int i = 0; i < __eFrustrumPlane_MAX__; i++) {
		normal = ZobVector3(m_frustrumPlanes[i].a, m_frustrumPlanes[i].b, m_frustrumPlanes[i].c);
		
		// X axis 
		if (normal.x > 0)
		{
			vmin.x = aabbMin->x;
			vmax.x = aabbMax->x;
		}
		else 
		{
			vmin.x = aabbMax->x;
			vmax.x = aabbMin->x;
		}
		// Y axis 
		if (normal.y > 0) 
		{
			vmin.y = aabbMin->y;
			vmax.y = aabbMax->y;
		}
		else 
		{
			vmin.y = aabbMax->y;
			vmax.y = aabbMin->y;
		}
		// Z axis 
		if (normal.z > 0) 
		{
			vmin.z = aabbMin->z;
			vmax.z = aabbMax->z;
		}
		else 
		{
			vmin.z = aabbMax->z;
			vmax.z = aabbMin->z;
		}
		//normal.Mul(-1);
		if (ZobVector3::Dot(&normal, &vmax) + m_frustrumPlanes[i].d < 0)
			return OUTSIDE;
		if (ZobVector3::Dot(&normal, &vmin) + m_frustrumPlanes[i].d <= 0)
			ret = INTERSECT;
	}
	return ret;
}

void Camera::UpdateViewProjectionMatrix(/*const ZobVector3* eyeV*/)
{
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);
	//SetViewMatrix(&m_left, &m_up, &m_forward, GetWorldPosition());
	ZobMatrix4x4::InvertMatrix4(m_modelMatrix, m_viewRotMatrix);
}

void Camera::UpdateViewProjectionMatrix(const ZobVector3* eyeV, const ZobVector3* targetV, const ZobVector3* upV)
{
	//g_update_camera_mutex.lock();
	m_viewTransaltion = GetWorldPosition();
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);	
	ZobVector3 up = upV;
	ZobVector3 forward = targetV;
	forward.x -= eyeV->x;
	forward.y -= eyeV->y;
	forward.z -= eyeV->z;
	forward.Normalize();
	ZobVector3 left = ZobVector3::Cross(&up, &forward);
	left.Normalize();
	up = ZobVector3::Cross(&forward, &left);
	up.Normalize();
	SetViewMatrix(&left, &up, &forward, eyeV);
	m_left = left;
	m_forward = forward;
	m_up = up;
}

void Camera::UpdateViewProjectionMatrix(const ZobVector3* eye, const float pitch, const float yaw)
{
	BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
	setProjectionMatrix(m_fov, b->width, b->height, b->zNear, b->zFar);

	float cosPitch = cos(pitch);
	float sinPitch = sin(pitch);
	float cosYaw = cos(yaw);
	float sinYaw = sin(yaw);

	ZobVector3 left = ZobVector3( cosYaw, 0, -sinYaw );
	ZobVector3 up = ZobVector3( sinYaw * sinPitch, cosPitch, cosYaw * sinPitch );
	ZobVector3 forward = ZobVector3( sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw );

	SetViewMatrix(&left, &up, &forward, eye);
	m_left = left;
	m_forward = forward;
	m_up = up;
}

void Camera::SetViewMatrix(const ZobVector3& left, const ZobVector3& up, const ZobVector3& fw, const ZobVector3& p)
{
	float x = -ZobVector3::Dot(&left, &p);
	float y = -ZobVector3::Dot(&up, &p);
	float z = -ZobVector3::Dot(&fw, &p);
	m_viewRotMatrix.SetData(0, 0, left.x);
	m_viewRotMatrix.SetData(0, 1, left.y);
	m_viewRotMatrix.SetData(0, 2, left.z);
	m_viewRotMatrix.SetData(0, 3, x);//-p.x);

	m_viewRotMatrix.SetData(1, 0, up.x);
	m_viewRotMatrix.SetData(1, 1, up.y);
	m_viewRotMatrix.SetData(1, 2, up.z);
	m_viewRotMatrix.SetData(1, 3, y);//-p.y);

	m_viewRotMatrix.SetData(2, 0, fw.x);
	m_viewRotMatrix.SetData(2, 1, fw.y);
	m_viewRotMatrix.SetData(2, 2, fw.z);
	m_viewRotMatrix.SetData(2, 3, z);//-p.z);

	m_viewRotMatrix.SetData(3, 0, 0);
	m_viewRotMatrix.SetData(3, 1, 0);
	m_viewRotMatrix.SetData(3, 2, 0);
	m_viewRotMatrix.SetData(3, 3, 1);
	//-------------------------------------------------------
	ZobMatrix4x4::InvertMatrix4(m_viewRotMatrix, m_invViewMatrix);
}

void Camera::setProjectionMatrix(const float angleOfView, const float width, const float height, const float zNear, const float zFar)
{
	const float ar = width / height;
	const float zRange = zFar- zNear;
	const float tanHalfFOV = -tanf(angleOfView / 2.0 * M_PI / 180.0);

	float a = 1.0f / (tanHalfFOV * ar);
	float b = 1.0f / tanHalfFOV;
	float c = (zNear + zFar) / zRange;
	float d = -2.0f * zFar * zNear / zRange;
	float e = 1.0f;

	m_projMatrix.SetData(0, 0, a);
	m_projMatrix.SetData(0, 1, 0.0f);
	m_projMatrix.SetData(0, 2, 0.0f);
	m_projMatrix.SetData(0, 3, 0.0f);

	m_projMatrix.SetData(1, 0, 0.0f);
	m_projMatrix.SetData(1, 1, b);
	m_projMatrix.SetData(1, 2, 0.0f);
	m_projMatrix.SetData(1, 3, 0.0f);

	m_projMatrix.SetData(2, 0, 0.0f);
	m_projMatrix.SetData(2, 1, 0.0f);
	m_projMatrix.SetData(2, 2, c);
	m_projMatrix.SetData(2, 3, d);

	m_projMatrix.SetData(3, 0, 0.0f);
	m_projMatrix.SetData(3, 1, 0.0f);
	m_projMatrix.SetData(3, 2, e);
	m_projMatrix.SetData(3, 3, 0.0f);

	//ZobMatrix4x4::InvertMatrix4(m_projMatrix, m_invProjectionMatrix);

	m_invProjectionMatrix.SetData(0, 0, 1.0f / a);
	m_invProjectionMatrix.SetData(0, 1, 0.0f);
	m_invProjectionMatrix.SetData(0, 2, 0.0f);
	m_invProjectionMatrix.SetData(0, 3, 0.0f);
	
	m_invProjectionMatrix.SetData(1, 0, 0.0f);
	m_invProjectionMatrix.SetData(1, 1, 1.0f / b);
	m_invProjectionMatrix.SetData(1, 2, 0.0f);
	m_invProjectionMatrix.SetData(1, 3, 0.0f);
	
	m_invProjectionMatrix.SetData(2, 0, 0.0f);
	m_invProjectionMatrix.SetData(2, 1, 0.0f);
	m_invProjectionMatrix.SetData(2, 2, 0.0f);
	m_invProjectionMatrix.SetData(2, 3, 1.0f / e);
	
	m_invProjectionMatrix.SetData(3, 0, 0.0f);
	m_invProjectionMatrix.SetData(3, 1, 0.0f);
	m_invProjectionMatrix.SetData(3, 2, 1.0f / d);
	m_invProjectionMatrix.SetData(3, 3, -c / (d*e) );

	ZobMatrix4x4::InvertMatrix4(GetViewMatrix(), m_invViewMatrix);
}

// x in [-1,1], y in [-1,1}
DirectZobType::Ray Camera::From2DToWorld(float x, float y) const
{
	DirectZobType::Ray r;
	//if(fabsf(x)<=1.0f && fabsf(y)<=1.0f)
	{
		BufferData* b = DirectZob::GetInstance()->GetEngine()->GetBufferData();
		static float pouet = 0.f;
		ZobVector3 v = ZobVector3(x, y, pouet);
		static bool mytest = true;
		m_invProjectionMatrix.Mul(&v);
		m_invViewMatrix.Mul(&v);
		v.x /= v.w;
		v.y /= v.w;
		v.z /= v.w;
		v.w = 1.0f;
		r.p = v;
		v = v - this->GetWorldPosition();
		v.Normalize();
		r.n = v;
	}
	return r;
}

ZobVector3 Camera::From2DToWorldOnPlane(const float x, const float y, const ZobVector3* p0, const ZobVector3* pn) const
{
	ZobVector3 ret = ZobVector3(0, 0, 0);
	DirectZobType::Ray r = From2DToWorld(x, y);
	ret = ZobGeometryHelper::LinePlaneIntersection(p0, pn, &r.p, &r.n);
	return ret;
}

void Camera::Rotate(float x, float y, float z)
{ 
	m_zobCameraController->Rotate(x, y, z); 
}

TiXmlNode* Camera::SaveUnderNode(TiXmlNode* node)
{
	
	TiXmlNode* n = ZobObject::SaveUnderNode(node);
	//m_varExposer->SaveUnderNode(n);
	return n;
}
