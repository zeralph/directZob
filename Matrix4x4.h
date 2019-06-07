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
	void Mul(Vector3* v) const;
	void SetScale(const float x, const float y, const float z);
	void SetRotation(const float degrees);
	void SetTranslation(const float x, const float y, const float z);
	Vector3 GetScale() const { return Vector3(m_data[0][0], m_data[1][1], m_data[2][2]); }
	Vector3 GetTranslation() const { return Vector3{ m_data[0][3], m_data[1][3], m_data[2][3] }; }
	inline float GetValue(const int i, const int j) const { return m_data[i][j]; }

private:
	float m_data[4][4];
};
