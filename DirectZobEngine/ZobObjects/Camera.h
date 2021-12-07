#pragma once
#include "../Rendering/ZobMatrix4x4.h"
#include "ZobObject.h"
#include <string>
#include "tinyxml.h"


class ZobCameraController;
class ZobCameraControllerOrbital;
class ZobCameraControllerFPS;
class ZobCameraControllerFollowCar;
class Camera : public ZobObject
{
friend class ZobCameraController;
friend class ZobCameraControllerOrbital;
friend class ZobCameraControllerFPS;
friend class ZobCameraControllerFollowCar;
public:
	enum eCameraType
	{
		eCamera_unset = 0,
		eCamera_base = 1,
		eCamera_revolving = 2,
		eCamera_orbital = 3,
		eCamera_fps = 4,
		eCamera_follow = 5,
		eCamera_followCar = 6,
		eCamera_orbital_free = 7,
		__eCameraType_MAX__= 8
	};

	enum eFrustrumPlanes
	{
		eFrustrumPlaneLeft=0,
		eFrustrumPlaneRight = 1,
		eFrustrumPlaneBottom = 2,
		eFrustrumPlaneTop = 3,
		eFrustrumPlaneNear = 5,
		eFrustrumPlaneFar = 4,		
		__eFrustrumPlane_MAX__ = 6
	};

	Camera(ZOBGUID::ZobType zobType, const std::string& name, eCameraType type, float fov, BufferData* bufferData, ZobObject* parent);
	Camera(std::string id, TiXmlElement* node, ZobObject* parent);
	~Camera() override;

	//void					Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix) override;
	void					Update(float dt) override;
	void					PreUpdate(float dt) override;
	void					Init() override;
	void					PostUpdate() override;
	void					DrawGizmos(const Camera* camera, Engine* engine) override;
	TiXmlNode*				SaveUnderNode(TiXmlNode* node) override;
	void					UpdateViewProjectionMatrix(/*const ZobVector3* eyeV*/);
	void					UpdateViewProjectionMatrix(const ZobVector3 * eyeV, const ZobVector3* targetV, const ZobVector3* upV);
	void					UpdateViewProjectionMatrix(const ZobVector3* eye, const float pitch, const float yaw);
	inline const ZobMatrix4x4* GetViewMatrix() const { return &m_viewRotMatrix; };
	inline const ZobMatrix4x4* GetProjectionMatrix() const { return &m_projMatrix; };
	inline float			GetFov() const { return m_fov; };
	inline void				SetFov(float fov) { m_fov = fov; };
	void					Move(float x, float y, float z);
	void					RotateOrbital(ZobVector3* center, float x, float y, float dist);
	void					SetTarget(const ZobVector3* t);
	void					RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle, bool recomputeVectors);
	void					Move(float dx, float dz, float dy, bool moveTargetVector);
	void					Zoom(float z);
	void					Rotate(float x, float y, float z);
	void					SetActive(bool b) { m_active = b; }
	DirectZobType::Ray		From2DToWorld(float x, float y) const;
	const Plane*			GetFrustrumPlanes() const { return &m_frustrumPlanes[0]; };
	ZobVector3				From2DToWorldOnPlane(const float x, const float y, const ZobVector3* p0, const ZobVector3* pn) const;
	bool					ClipSegmentToFrustrum(ZobVector3* p1, ZobVector3* p2, float& outP2Factor) const;
	static bool				ClipSegmentToPlanes(ZobVector3* p1, ZobVector3* p2, const DirectZobType::Plane* planes, float& outP2Factor);
	Camera::eFrustrumPlanes	PointIsInFrustrum(const ZobVector3* pt) const;
	bool					AABBIsInFrustrum(const ZobVector3* aabbMin, const ZobVector3* aabbMax) const;
	void					ChangeCameraController(eCameraType newType);
	inline void				ToViewSpace(ZobVector3* v) const
	{
		m_viewRotMatrix.Mul(v);
	};
	inline void				ToProjectedSpace(ZobVector3* vp)
	{
		m_projMatrix.Mul(vp);
	}
	inline void				ProjectPointFromWorld(ZobVector3* wp)
	{
		ToViewSpace(wp);
		ToProjectedSpace(wp);
	}

private:
	void					SetViewMatrix(const ZobVector3 &left, const ZobVector3 &up, const ZobVector3 &fw, const ZobVector3 &p);
	void					setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);
	void					RecomputeFLUVectors(const ZobVector3* forwardV, const ZobVector3* upV);
	void					RecomputeFrustrumPlanes();
	void					NormalizePlane(Plane& plane);
	void					WrapVariables();
	ZobMatrix4x4 m_viewRotMatrix;
	ZobMatrix4x4 m_projMatrix;
	float m_fov;
	ZobVector3 m_viewTransaltion;
	ZobMatrix4x4 m_invProjectionMatrix;
	ZobMatrix4x4 m_invViewMatrix;
	Plane m_frustrumPlanes[6];
	ZobCameraController* m_zobCameraController;
	eCameraType m_controlerType;
	eCameraType m_nextControlerType;
	float m_active;
};

