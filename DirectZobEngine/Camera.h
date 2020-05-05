#pragma once
#include "Matrix4x4.h"
#include "ZobObject.h"
#include <string>


class Camera : public ZobObject
{
public:
	Camera(const std::string& name, Vector3& position, Vector3& target, Vector3& up, float fov, BufferData* bufferData);
	~Camera();

	void Update();

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}
	inline void SetPosition(const Vector3* p) { m_cameraPosition = p;/*m_viewMatrix.SetTranslation(-p->x, -p->y, -p->z);*/ }
	inline void SetRotation(const Vector3* p) { m_viewMatrix.SetRotation(-p->x, -p->y, -p->z); }
	inline const Vector3* GetPosition() const { return &m_cameraPosition; }
	inline const Vector3* GetTarget() const { return &m_cameraTarget; }
	inline const Vector3* GetForward() const { return &m_cameraFw; }
	inline const std::string& GetName() const { return m_name; }
	void SetLookAt(const Vector3* from, const Vector3* to, const Vector3* up);
	void RotateAroundAxis(float dx, float dy);
	void Move(float dx, float dy);
	void Zoom(float z);

private:
	void setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);
	void UpdateLookAt();
	void InitView();
	void RecomputeVectors();


	std::string m_name;
	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projMatrix;
	Vector3 m_cameraPosition;
	Vector3 m_cameraTarget;
	Vector3 m_cameraUp;
	Vector3 m_cameraFw;
	Vector3 m_cameraLeft;
	float m_cameraZoom;
	float m_fov;
	bool m_shiftPressed;
	bool m_mouseLeftButtonPressed;
	bool m_mouseRightButtonPressed;
	int m_mouseX;
	int m_mouseY;
};

