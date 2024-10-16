#include "ZobMatrix2x2.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static ZobMatrix2x2 tmp;
static ZobMatrix2x2 tmpMul;
static float identityArray[9] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
ZobMatrix2x2::ZobMatrix2x2()
{
	Identity();
}

ZobMatrix2x2::ZobMatrix2x2(const ZobMatrix2x2* m)
{
	CopyFrom(m);
}

ZobMatrix2x2::~ZobMatrix2x2()
{
}

void ZobMatrix2x2::Mul(const ZobMatrix2x2* m)
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

void ZobMatrix2x2::Identity()
{
	memcpy(&m_data, &identityArray, sizeof(identityArray));
}

void ZobMatrix2x2::CopyFrom(const ZobMatrix2x2* m)
{
	memcpy(&m_data, &m->m_data, sizeof(m_data));
}

void ZobMatrix2x2::Mul(ZobVector2 * v) const
{
	float fx = m_data[0][0] * v->x + m_data[0][1] * v->y + m_data[0][2] * v->w;
	float fy = m_data[1][0] * v->x + m_data[1][1] * v->y + m_data[1][2] * v->w;
	float fw = m_data[2][0] * v->x + m_data[2][1] * v->y + m_data[2][2] * v->w;
	v->x = fx;// +m_translationX;
	v->y = fy;// +m_translationY;
	v->w = fw;
}

void ZobMatrix2x2::SetShear(const float x, const float y)
{
	tmp.Identity();
	tmp.m_data[0][1] = y;
	tmp.m_data[1][0] = x;
	Mul(&tmp);
}

void ZobMatrix2x2::SetScale(const float x, const float y)
{
	tmp.Identity();
	tmp.m_data[0][0] = x;
	tmp.m_data[1][1] = y;
	Mul(&tmp);
}

void ZobMatrix2x2::SetPosition(const float x, const float y)
{
	tmp.Identity();
	tmp.m_data[0][2] = x;
	tmp.m_data[1][2] = y;
	Mul(&tmp);
}

void ZobMatrix2x2::SetRotation(const float degrees)
{
	tmp.Identity();
	float r = degrees * M_PI / 180.0;
	tmp.m_data[0][0] = cos(r);
	tmp.m_data[0][1] = -sin(r);
	tmp.m_data[1][0] = sin(r);
	tmp.m_data[1][1] = cos(r);
	Mul(&tmp);
}