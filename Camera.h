#pragma once
#include "Matrix4x4.h"

class Camera
{
public:
	Camera();
	~Camera();

	void setProjectionMatrix(const float angleOfView, const float near, const float far);

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}

private:

	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projMatrix;
};

