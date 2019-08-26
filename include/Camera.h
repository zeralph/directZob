#pragma once
#include "Matrix4x4.h"

class Camera
{
public:
	Camera(const char* name, Vector3 position, Vector3 target, Vector3 up, float fov, BufferData* bufferData);
	~Camera();

	void Update();

	const Vector3* GetEyeVector() const { return &Vector3::Vector3Z; };
	inline const Matrix4x4* GetViewMatrix() const { return &m_viewMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}
	inline void SetPosition(const Vector3* p) { m_viewMatrix.SetTranslation(-p->x, -p->y, -p->z); }
	inline void SetRotation(const Vector3* p) { m_viewMatrix.SetRotation(-p->x, -p->y, -p->z); }
	inline const Vector3* GetPosition() { return &m_cameraPosition; }
	inline const Vector3* GetTarget() { return &m_cameraTarget; }
	inline const char* GetName() const { return m_name; }

private:
	void setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);
	void SetLookAt(const Vector3* from, const Vector3* to, const Vector3* tmp);
	void InitView();

	const char* m_name;
	BufferData* m_bufferData = NULL;
	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projMatrix;
	Vector3 m_cameraPosition;
	Vector3 m_cameraTarget;
	Vector3 m_cameraUp;
	float m_cameraZoom;
	float m_fov;
};

