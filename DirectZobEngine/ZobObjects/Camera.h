#pragma once
#include "../Rendering/ZobMatrix4x4.h"
#include "ZobObject.h"
#include <string>
#include "tinyxml.h"


class ZobCameraController;
class ZobCameraControllerOrbital;
class ZobCameraControllerFPS;
class Camera : public ZobObject
{
friend class ZobCameraController;
friend class ZobCameraControllerOrbital;
friend class ZobCameraControllerFPS;
public:
	enum eCameraType
	{
		eCamera_base=0,
		eCamera_revolving=1,
		eCamera_orbital = 2,
		eCamera_fps=3,
		eCamera_follow=4,
	};

	enum eTargetMode
	{
		eTarget_none=0,
		eTarget_Vector,
		eTarget_Object,
		eTarget_FPS,
		__eTarget_MAX__
	};

	enum eFrustrumPlanes
	{
		eFrustrumPlaneLeft=0,
		eFrustrumPlaneRight = 1,
		eFrustrumPlaneBottom = 2,
		eFrustrumPlaneTop = 3,
		eFrustrumPlaneNear = 5,
		eFrustrumPlaneFar = 4,		
	};

	Camera(ZOBGUID::Type zobType, const std::string& name, eCameraType type, float fov, BufferData* bufferData, ZobObject* parent);
	Camera(ulong id, TiXmlElement* node, ZobObject* parent);
	~Camera() override;

	//void					Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix) override;
	void					Update() override;
	void					PreUpdate() override;
	void					UpdateAfter();
	void					DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*				SaveUnderNode(TiXmlNode* node) override;
	void					UpdateViewProjectionMatrix(/*const ZobVector3* eyeV*/);
	void					UpdateViewProjectionMatrix(const ZobVector3 * eyeV, const ZobVector3* targetV, const ZobVector3* upV);
	void					UpdateViewProjectionMatrix(const ZobVector3* eye, const float pitch, const float yaw);
	inline const ZobMatrix4x4* GetViewMatrix() const { return &m_viewRotMatrix; };
	inline const ZobMatrix4x4* GetProjectionMatrix() const { return &m_projMatrix; };
	inline float			GetFov() const { return m_fov; };
	inline void				SetFov(float fov) { m_fov = fov; };

	void					SetType(eCameraType type);
	
	void					SetTarget(const ZobVector3 t) { m_tagetMode = eTarget_Vector; m_targetVector = t; };
	void					SetTarget(const ZobObject* z) { m_tagetMode = eTarget_Object; m_targetObject = z; };
	void					SetNoTarget() { m_tagetMode = eTarget_none; };
	bool					GetTargetVector(ZobVector3* t);
	void					RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle, bool recomputeVectors);
	void					Move(float dx, float dz, float dy, bool moveTargetVector);
	void					Zoom(float z);
	void					Rotate(float x, float y, float z);
	void					SetActive(bool b) { m_active = b; }
	DirectZobType::Ray		From2DToWorld(float x, float y);
	const Plane*			GetFrustrumPlanes() const { return &m_frustrumPlanes[0]; };
	bool					From2DToWorldOnPlane(const float x, const float y, const ZobVector3* p0, const ZobVector3* pn, ZobVector3* ret);
	bool					ClipSegmentToFrustrum(ZobVector3* p1, ZobVector3* p2, bool bCheckOnly) const;
	static bool				ClipSegmentToPlanes(ZobVector3* p1, ZobVector3* p2, bool bCheckOnly, const DirectZobType::Plane* planes);
	bool					ClipSegmentToNearPlane(ZobVector3* p1, ZobVector3* p2, bool bCheckOnly) const;
	bool					PointIsInFrustrum(const ZobVector3* pt) const;
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

	ZobMatrix4x4 m_viewRotMatrix;
	ZobMatrix4x4 m_projMatrix;
	float m_fov;
	ZobVector3 m_viewTransaltion;
	eTargetMode m_tagetMode;
	ZobVector3 m_targetVector;
	const ZobObject* m_targetObject;
	//ZobMatrix4x4 m_invModelMatrix;
	ZobMatrix4x4 m_invProjectionMatrix;
	ZobMatrix4x4 m_invViewMatrix;
	//ZobVector3 m_nextTranslation;
	Plane m_frustrumPlanes[6];
	ZobCameraController* m_zobCameraController;
	float m_active;
};

