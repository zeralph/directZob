#pragma once
#include "Matrix4x4.h"
#include "ZobObject.h"
#include <string>


class Camera : public ZobObject
{
public:
	Camera(const std::string& name, float fov, BufferData* bufferData, ZobObject* parent);
	~Camera();

	void Update(const Matrix4x4& parentMatrix, const Matrix4x4& parentRSMatrix);

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewRotMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}
	inline const Vector3* GetForward() const 
	{
		return &m_forward;
		Vector3 v;
		v.x = m_viewRotMatrix.GetValue(0,1);
		v.y = m_viewRotMatrix.GetValue(1, 1);
		v.z = m_viewRotMatrix.GetValue(2, 1);
		v.Normalize();
		return &v; 
	}
	bool SetTarget(const Vector3* t);
	void Draw(const Camera* camera, Core::Engine* engine);
	void RotateAroundAxis(const Vector3* axis, float dx, float dy);
	void Move(float dx, float dy);
	void Zoom(float z);
	inline void ToViewSpace(Vector3* v) const
	{
		v->x -= m_translation.x;
		v->y -= m_translation.y;
		v->z -= m_translation.z;
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
};

