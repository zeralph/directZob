#pragma once
#include "Types.h"
#include "Vector3.h"

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4* m);
	~Matrix4x4();

	void Identity();
	void CopyFrom(const Matrix4x4* m);
	void Mul(const Matrix4x4* m);
	inline void Mul(Vector3* v) const
	{
		float fx = m_data[0][0] * v->x + m_data[0][1] * v->y + m_data[0][2] * v->z + m_data[0][3] * v->w;
		float fy = m_data[1][0] * v->x + m_data[1][1] * v->y + m_data[1][2] * v->z + m_data[1][3] * v->w;
		float fz = m_data[2][0] * v->x + m_data[2][1] * v->y + m_data[2][2] * v->z + m_data[2][3] * v->w;
		float fw = m_data[3][0] * v->x + m_data[3][1] * v->y + m_data[3][2] * v->z + m_data[3][3] * v->w;
		v->x = fx;// +m_translationX;
		v->y = fy;// +m_translationY;
		v->z = fz;
		v->w = fw;
	};
	void FromVectors(const Vector3& left, const Vector3& up, const Vector3& forward);
	inline void SetData(uint i, uint j, float f) { m_data[i][j] = f; }
	void SetScale(const float x, const float y, const float z);
	void SetRotation(const float x, const float y, const float z);
	void SetTranslation(const float x, const float y, const float z);
	void SetScale(const Vector3& v);
	void AddScale(const Vector3& v);
	void SetRotation(const Vector3& v);
	void AddRotation(const Vector3& v);
	void SetTranslation(const Vector3& v);
	void AddTranslation(const Vector3& v);
	Vector3 GetRotation() const;
	Vector3 GetScale() const { return Vector3(m_data[0][0], m_data[1][1], m_data[2][2]); }
	Vector3 GetTranslation() const { return Vector3{ m_data[0][3], m_data[1][3], m_data[2][3] }; }
	inline float GetValue(const int i, const int j) const { return m_data[i][j]; }
	static void InvertMatrix4(const Matrix4x4& m, Matrix4x4& im);
	static Matrix4x4 RotateAroundAxis(const Vector3& axis, const float angle);
	//float& operator[](int i) {return &m_data[i][j]; };
private:

	void SetRotationX(const float r);
	void SetRotationY(const float r);
	void SetRotationZ(const float r);

	float m_data[4][4];
};
