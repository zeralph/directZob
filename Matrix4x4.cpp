#include "Matrix4x4.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static Matrix4x4 tmp;
static Matrix4x4 tmpMul;
static float identityArray[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
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
	//tmpMul.Identity();
	tmpMul.m_data[0][0] = (m_data[0][0] * m->m_data[0][0]) +
		(m_data[0][1] * m->m_data[1][0]) +
		(m_data[0][2] * m->m_data[2][0]);

	tmpMul.m_data[1][0] = (m_data[1][0] * m->m_data[0][0]) +
		(m_data[1][1] * m->m_data[1][0]) +
		(m_data[1][2] * m->m_data[2][0]);

	tmpMul.m_data[2][0] = (m_data[2][0] * m->m_data[0][0]) +
		(m_data[2][1] * m->m_data[1][0]) +
		(m_data[2][2] * m->m_data[2][0]);


	tmpMul.m_data[0][1] = (m_data[0][0] * m->m_data[0][1]) +
		(m_data[0][1] * m->m_data[1][1]) +
		(m_data[0][2] * m->m_data[2][1]);

	tmpMul.m_data[1][1] = (m_data[1][0] * m->m_data[0][1]) +
		(m_data[1][1] * m->m_data[1][1]) +
		(m_data[1][2] * m->m_data[2][1]);

	tmpMul.m_data[2][1] = (m_data[2][0] * m->m_data[0][1]) +
		(m_data[2][1] * m->m_data[1][1]) +
		(m_data[2][2] * m->m_data[2][1]);


	tmpMul.m_data[0][2] = (m_data[0][0] * m->m_data[0][2]) +
		(m_data[0][1] * m->m_data[1][2]) +
		(m_data[0][2] * m->m_data[2][2]);

	tmpMul.m_data[1][2] = (m_data[1][0] * m->m_data[0][2]) +
		(m_data[1][1] * m->m_data[1][2]) +
		(m_data[1][2] * m->m_data[2][2]);

	tmpMul.m_data[2][2] = (m_data[2][0] * m->m_data[0][2]) +
		(m_data[2][1] * m->m_data[1][2]) +
		(m_data[2][2] * m->m_data[2][2]);

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

void Matrix4x4::Mul(Vector3* v) const
{
	float fx = m_data[0][0] * v->x + m_data[0][1] * v->y + m_data[0][2] * v->w;
	float fy = m_data[1][0] * v->x + m_data[1][1] * v->y + m_data[1][2] * v->w;
	float fw = m_data[2][0] * v->x + m_data[2][1] * v->y + m_data[2][2] * v->w;
	v->x = fx;// +m_translationX;
	v->y = fy;// +m_translationY;
	v->w = fw;
}

void Matrix4x4::SetScale(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][0] = x;
	tmp.m_data[1][1] = y;
	tmp.m_data[2][2] = z;
	Mul(&tmp);
}

void Matrix4x4::SetTranslation(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][2] = x;
	tmp.m_data[1][2] = y;
	tmp.m_data[2][3] = z;
	Mul(&tmp);
}

void Matrix4x4::SetRotation(const float degrees)
{
	tmp.Identity();
	float r = degrees * M_PI / 180.0;
	tmp.m_data[0][0] = cos(r);
	tmp.m_data[0][1] = -sin(r);
	tmp.m_data[1][0] = sin(r);
	tmp.m_data[1][1] = cos(r);
	Mul(&tmp);
}