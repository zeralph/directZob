#pragma once
#include "Matrix4x4.h"
#include "ZobObject.h"
#include <string>


class Camera : public ZobObject
{
public:

	enum eTargetMode
	{
		eTarget_none=0,
		eTarget_Vector,
		eTarget_Object,
		__eTarget_MAX__
	};
	Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent);
	~Camera();

	void Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix);
	void UpdateViewProjectionMatrix();

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewRotMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}
	inline const Vector3* GetForward() const 
	{
		//return &m_forward;
		Vector3 v;
		v.x = m_viewRotMatrix.GetValue(0,1);
		v.y = m_viewRotMatrix.GetValue(1, 1);
		v.z = m_viewRotMatrix.GetValue(2, 1);
		v.Normalize();
		return &v; 
	}
	void SetTarget(const Vector3 t) { m_tagetMode = eTarget_Vector; m_targetVector = t; }
	void SetTarget(const ZobObject* z) { m_tagetMode = eTarget_Object; m_targetObject = z; }
	void SetNoTarget() { m_tagetMode = eTarget_none; }
	void Draw(const Camera* camera, Core::Engine* engine);
	void RotateAroundAxis(const Vector3* axis, float angle);
	void Move(float dx, float dy, bool moveTargetVector);
	void Zoom(float z);
	inline void ToViewSpace(Vector3* v) const
	{
		v->x -= m_viewTransaltion.x;
		v->y -= m_viewTransaltion.y;
		v->z -= m_viewTransaltion.z;
		m_viewRotMatrix.Mul(v);
	}
private:
	void SetViewMatrix(const Vector3 &left, const Vector3 &up, const Vector3 &fw, const Vector3 &p);
	void setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);

	Vector3 m_left;
	Vector3 m_forward;
	Vector3 m_up;
	Matrix4x4 m_viewRotMatrix;
	Matrix4x4 m_projMatrix;
	float m_fov;
	Vector3 m_viewTransaltion;
	eTargetMode m_tagetMode;
	Vector3 m_targetVector;
	const ZobObject* m_targetObject;
};

