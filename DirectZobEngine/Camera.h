#pragma once
#include "ZobMatrix4x4.h"
#include "ZobObject.h"
#include <string>
#include "tinyxml.h"

class Camera : public ZobObject
{
public:

	class Ray
	{
	public:
		ZobVector3 p;
		ZobVector3 n;
	};

	enum eTargetMode
	{
		eTarget_none=0,
		eTarget_Vector,
		eTarget_Object,
		eTarget_FPS,
		__eTarget_MAX__
	};
	Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent);
	Camera(TiXmlElement* node, ZobObject* parent);
	~Camera() override;

	//void					Update(const ZobMatrix4x4& parentMatrix, const ZobMatrix4x4& parentRSMatrix) override;
	void					Update() override;
	void					DrawGizmos(const Camera* camera, Core::Engine* engine) override;
	TiXmlNode*				SaveUnderNode(TiXmlNode* node) override;
	void					UpdateViewProjectionMatrix(const ZobVector3* eyeV);
	void					UpdateViewProjectionMatrix(const ZobVector3 * eyeV, const ZobVector3* targetV, const ZobVector3* upV);
	void					UpdateViewProjectionMatrix(const ZobVector3* eye, const float pitch, const float yaw);
	inline const ZobMatrix4x4* GetViewMatrix() const { return &m_viewRotMatrix; };
	inline const ZobMatrix4x4* GetProjectionMatrix() const { return &m_projMatrix; };
	inline float			GetFov() const { return m_fov; };
	void					SetTarget(const ZobVector3 t) { m_tagetMode = eTarget_Vector; m_targetVector = t; };
	void					SetTarget(const ZobObject* z) { m_tagetMode = eTarget_Object; m_targetObject = z; };
	void					SetNoTarget() { m_tagetMode = eTarget_none; };
	bool					GetTargetVector(ZobVector3* t);
	
	void					RotateAroundPointAxis(const ZobVector3* point, const ZobVector3* axis, const ZobVector3* lockAxis, float angle, bool recomputeVectors);
	void					Move(float dx, float dy, bool moveTargetVector);
	void					Zoom(float z);
	Ray						From2DToWorld(const ZobVector3* v2d);
	void					From2DToWorldOnPlane(ZobVector3* v2d, ZobVector3* p0, ZobVector3* pn);
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
	ZobMatrix4x4 m_viewRotMatrix;
	ZobMatrix4x4 m_projMatrix;
	float m_fov;
	ZobVector3 m_viewTransaltion;
	eTargetMode m_tagetMode;
	ZobVector3 m_targetVector;
	const ZobObject* m_targetObject;
	ZobMatrix4x4 m_invModelMatrix;
	ZobMatrix4x4 m_invProjectionMatrix;
	ZobMatrix4x4 m_invViewMatrix;
	//ZobVector3 m_nextTranslation;
};

