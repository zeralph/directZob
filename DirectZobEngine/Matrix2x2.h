#pragma once
#include "Types.h"
#include "Vector2.h"

class Matrix2x2
{
public:
	Matrix2x2();
	Matrix2x2(const Matrix2x2* m);
	~Matrix2x2();

	void Identity();
	void CopyFrom(const Matrix2x2* m);
	void Mul(const Matrix2x2* m);
	void Mul(Vector2 * v) const;
	void SetScale(const float x, const float y);
	void SetRotation(const float degrees);
	void SetShear(const float x, const float y);
	void SetTranslation(const float x, const float y);
	Vector2 GetScale() const { return Vector2(m_data[0][0], m_data[1][1]); }
	Vector2 GetTranslation() const { return Vector2{ m_data[0][2], m_data[1][2] }; }
	inline float 	GetValue(const int i, const int j) const { return m_data[i][j]; }

private:
	float 			m_data[3][3];
};

