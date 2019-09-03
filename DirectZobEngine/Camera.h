#pragma once
#include "Matrix4x4.h"
#include "MiniFB_enums.h"
#include <string>
class Camera
{
public:
	Camera(const std::string& name, Vector3& position, Vector3& target, Vector3& up, float fov, BufferData* bufferData);
	~Camera();

	void Update();

	inline const Matrix4x4* GetViewMatrix() const { return &m_viewMatrix; }
	inline const Matrix4x4* GetProjectionMatrix() const {return &m_projMatrix;}
	inline void SetPosition(const Vector3* p) { m_viewMatrix.SetTranslation(-p->x, -p->y, -p->z); }
	inline void SetRotation(const Vector3* p) { m_viewMatrix.SetRotation(-p->x, -p->y, -p->z); }
	inline const Vector3* GetPosition() const { return &m_cameraPosition; }
	inline const Vector3* GetTarget() const { return &m_cameraTarget; }
	inline const Vector3* GetForward() const { return &m_cameraFw; }
	inline const std::string& GetName() const { return m_name; }

	void OnMouseScroll(float deltaY);
	void OnKeyboardInput(Key key, bool isPressed);
	void OnMouseMove(int x, int y);
	void OnMouseButton(MouseButton button, bool isPressed);

private:
	void setProjectionMatrix(const float angleOfView, const float width, const float height, const float near, const float far);
	void SetLookAt(const Vector3* from, const Vector3* to, const Vector3* tmp);
	void InitView();
	void RecomputeVectors();


	std::string m_name;
	BufferData* m_bufferData = NULL;
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

