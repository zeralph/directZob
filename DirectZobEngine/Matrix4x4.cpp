#include "Matrix4x4.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static Matrix4x4 tmp;
static Matrix4x4 tmpMul;
static float identityArray[16] = {	1.0f, 0.0f, 0.0f, 0.0f, 
									0.0f, 1.0f, 0.0f, 0.0f, 
									0.0f, 0.0f, 1.0f, 0.0f, 
									0.0f, 0.0f, 0.0f, 1.0f };
Matrix4x4::Matrix4x4()
{
	Identity();
}

Matrix4x4::Matrix4x4(const Matrix4x4* m)
{
	CopyFrom(m);
}

Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::Mul(const Matrix4x4* m)
{
	tmpMul.Identity();
	for (int n = 0; n < 4; n++) {
		for (int p = 0; p < 4; p++) {
			float num = 0;
			for (int u = 0; u < 4; u++) {
				num += m_data[n][u] * m->m_data[u][p];
			}
			tmpMul.m_data[n][p] = num;
		}
	}
	memcpy(&m_data, &tmpMul.m_data, sizeof(tmpMul.m_data));
}

void Matrix4x4::Identity()
{
	memcpy(&m_data, &identityArray, sizeof(identityArray));
}

void Matrix4x4::CopyFrom(const Matrix4x4* m)
{
	memcpy(&m_data, &m->m_data, sizeof(m_data));
}

void Matrix4x4::AddScale(const Vector3& v)
{
	m_data[0][0] *= v.x;
	m_data[1][1] *= v.y;
	m_data[2][2] *= v.z;
}

void Matrix4x4::SetScale(const Vector3& v)
{
	tmp.Identity();
	tmp.m_data[0][0] = v.x;
	tmp.m_data[1][1] = v.y;
	tmp.m_data[2][2] = v.z;
	Mul(&tmp);
}

void Matrix4x4::SetScale(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][0] = x;
	tmp.m_data[1][1] = y;
	tmp.m_data[2][2] = z;
	Mul(&tmp);
}

void Matrix4x4::SetTranslation(const Vector3& v)
{
	tmp.Identity();
	tmp.m_data[0][3] = v.x;
	tmp.m_data[1][3] = v.y;
	tmp.m_data[2][3] = v.z;
	Mul(&tmp);
}

Vector3 Matrix4x4::GetRotation() const
{
	float heading = 0.0f;
	float bank = 0.0f;
	float attitude = 0.0f;

	if (m_data[1][0] > 0.998) { // singularity at north pole
		heading = atan2(m_data[0][2], m_data[2][2]);
		attitude = M_PI / 2;
		bank = 0;
	}
	else if (m_data[1][0] < -0.998) { // singularity at south pole
		heading = atan2(m_data[0][2], m_data[2][2]);
		attitude = -M_PI / 2;
		bank = 0;
	}
	else
	{
		heading = atan2(-m_data[2][0], m_data[0][0]);
		bank = atan2(-m_data[1][2], m_data[1][1]);
		attitude = asin(m_data[1][0]);
	}
	return Vector3(heading, bank, attitude);
}

void Matrix4x4::AddRotation(const Vector3& v)
{
	Vector3 r = GetRotation();
	SetRotationX(r.x + v.x);
	SetRotationY(r.y + v.y);
	SetRotationZ(r.z + v.z);
}


void Matrix4x4::AddTranslation(const Vector3& v)
{
	m_data[0][3] += v.x;
	m_data[1][3] += v.y;
	m_data[2][3] += v.z;
}

void Matrix4x4::SetTranslation(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][3] = x;
	tmp.m_data[1][3] = y;
	tmp.m_data[2][3] = z;
	Mul(&tmp);
}

void Matrix4x4::SetRotation(const Vector3& v)
{
	SetRotationX(-v.x);
	SetRotationY(-v.y);
	SetRotationZ(-v.z);
}

void Matrix4x4::SetRotation(const float x, const float y, const float z)
{
	SetRotationX(-x);
	SetRotationY(-y);
	SetRotationZ(-z);
}

void Matrix4x4::SetRotationX(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[1][1] = (float)cos(rx);
	tmp.m_data[1][2] = (float)-sin(rx);
	tmp.m_data[2][1] = (float)sin(rx);
	tmp.m_data[2][2] = (float)cos(rx);
	Mul(&tmp);
}

void Matrix4x4::SetRotationY(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[0][0] = (float)cos(rx);
	tmp.m_data[0][2] = (float)sin(rx);
	tmp.m_data[2][0] = (float)-sin(rx);
	tmp.m_data[2][2] = (float)cos(rx);
	Mul(&tmp);
}

void Matrix4x4::SetRotationZ(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[0][0] = (float)cos(rx);
	tmp.m_data[0][1] = (float)-sin(rx);
	tmp.m_data[1][0] = (float)sin(rx);
	tmp.m_data[1][1] = (float)cos(rx);
	Mul(&tmp);
}

void Matrix4x4::InvertMatrix4(const Matrix4x4& m, Matrix4x4& im)
{
	float det;
	double A2323 = m.m_data[2][2] * m.m_data[3][3] - m.m_data[2][3] * m.m_data[3][2];
	double A1323 = m.m_data[2][1] * m.m_data[3][3] - m.m_data[2][3] * m.m_data[3][1];
	double A1223 = m.m_data[2][1] * m.m_data[3][2] - m.m_data[2][2] * m.m_data[3][1];
	double A0323 = m.m_data[2][0] * m.m_data[3][3] - m.m_data[2][3] * m.m_data[3][0];
	double A0223 = m.m_data[2][0] * m.m_data[3][2] - m.m_data[2][2] * m.m_data[3][0];
	double A0123 = m.m_data[2][0] * m.m_data[3][1] - m.m_data[2][1] * m.m_data[3][0];
	double A2313 = m.m_data[1][2] * m.m_data[3][3] - m.m_data[1][3] * m.m_data[3][2];
	double A1313 = m.m_data[1][1] * m.m_data[3][3] - m.m_data[1][3] * m.m_data[3][1];
	double A1213 = m.m_data[1][1] * m.m_data[3][2] - m.m_data[1][2] * m.m_data[3][1];
	double A2312 = m.m_data[1][2] * m.m_data[2][3] - m.m_data[1][3] * m.m_data[2][2];
	double A1312 = m.m_data[1][1] * m.m_data[2][3] - m.m_data[1][3] * m.m_data[2][1];
	double A1212 = m.m_data[1][1] * m.m_data[2][2] - m.m_data[1][2] * m.m_data[2][1];
	double A0313 = m.m_data[1][0] * m.m_data[3][3] - m.m_data[1][3] * m.m_data[3][0];
	double A0213 = m.m_data[1][0] * m.m_data[3][2] - m.m_data[1][2] * m.m_data[3][0];
	double A0312 = m.m_data[1][0] * m.m_data[2][3] - m.m_data[1][3] * m.m_data[2][0];
	double A0212 = m.m_data[1][0] * m.m_data[2][2] - m.m_data[1][2] * m.m_data[2][0];
	double A0113 = m.m_data[1][0] * m.m_data[3][1] - m.m_data[1][1] * m.m_data[3][0];
	double A0112 = m.m_data[1][0] * m.m_data[2][1] - m.m_data[1][1] * m.m_data[2][0];

	det = m.m_data[0][0] * (m.m_data[1][1] * A2323 - m.m_data[1][2] * A1323 + m.m_data[1][3] * A1223)
		- m.m_data[0][1] * (m.m_data[1][0] * A2323 - m.m_data[1][2] * A0323 + m.m_data[1][3] * A0223)
		+ m.m_data[0][2] * (m.m_data[1][0] * A1323 - m.m_data[1][1] * A0323 + m.m_data[1][3] * A0123)
		- m.m_data[0][3] * (m.m_data[1][0] * A1223 - m.m_data[1][1] * A0223 + m.m_data[1][2] * A0123);

	det = 1.0f / det;

	im.m_data[0][0] = det * (m.m_data[1][1] * A2323 - m.m_data[1][2] * A1323 + m.m_data[1][3] * A1223);
	im.m_data[0][1] = det * -(m.m_data[0][1] * A2323 - m.m_data[0][2] * A1323 + m.m_data[0][3] * A1223);
	im.m_data[0][2] = det * (m.m_data[0][1] * A2313 - m.m_data[0][2] * A1313 + m.m_data[0][3] * A1213);
	im.m_data[0][3] = det * -(m.m_data[0][1] * A2312 - m.m_data[0][2] * A1312 + m.m_data[0][3] * A1212);
	im.m_data[1][0] = det * -(m.m_data[1][0] * A2323 - m.m_data[1][2] * A0323 + m.m_data[1][3] * A0223);
	im.m_data[1][1] = det * (m.m_data[0][0] * A2323 - m.m_data[0][2] * A0323 + m.m_data[0][3] * A0223);
	im.m_data[1][2] = det * -(m.m_data[0][0] * A2313 - m.m_data[0][2] * A0313 + m.m_data[0][3] * A0213);
	im.m_data[1][3] = det * (m.m_data[0][0] * A2312 - m.m_data[0][2] * A0312 + m.m_data[0][3] * A0212);
	im.m_data[2][0] = det * (m.m_data[1][0] * A1323 - m.m_data[1][1] * A0323 + m.m_data[1][3] * A0123);
	im.m_data[2][1] = det * -(m.m_data[0][0] * A1323 - m.m_data[0][1] * A0323 + m.m_data[0][3] * A0123);
	im.m_data[2][2] = det * (m.m_data[0][0] * A1313 - m.m_data[0][1] * A0313 + m.m_data[0][3] * A0113);
	im.m_data[2][3] = det * -(m.m_data[0][0] * A1312 - m.m_data[0][1] * A0312 + m.m_data[0][3] * A0112);
	im.m_data[3][0] = det * -(m.m_data[1][0] * A1223 - m.m_data[1][1] * A0223 + m.m_data[1][2] * A0123);
	im.m_data[3][1] = det * (m.m_data[0][0] * A1223 - m.m_data[0][1] * A0223 + m.m_data[0][2] * A0123);
	im.m_data[3][2] = det * -(m.m_data[0][0] * A1213 - m.m_data[0][1] * A0213 + m.m_data[0][2] * A0113);
	im.m_data[3][3] = det * (m.m_data[0][0] * A1212 - m.m_data[0][1] * A0212 + m.m_data[0][2] * A0112);
}