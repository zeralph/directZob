#pragma once
#include "Matrix4x4.h"

class Camera
{
public:
	Camera();
	~Camera();

	void setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);

	const Vector3* GetEyeVector() const { return &Vector3::Vector3Z; };

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}

	inline void SetPosition(const Vector3* p) { m_viewMatrix.Identity(); m_viewMatrix.SetTranslation(p->x, p->y, p->z); }

private:

	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projMatrix;
};

