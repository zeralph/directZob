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
	SetRotationX(v.x);
	SetRotationY(v.y);
	SetRotationZ(v.z);
}

void Matrix4x4::SetRotation(const float x, const float y, const float z)
{
	SetRotationX(x);
	SetRotationY(y);
	SetRotationZ(z);
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