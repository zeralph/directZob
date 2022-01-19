#include "ZobMatrix4x4.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static ZobMatrix4x4 tmp;
static ZobMatrix4x4 tmpMul;
static float identityArray[16] = {	1.0f, 0.0f, 0.0f, 0.0f, 
									0.0f, 1.0f, 0.0f, 0.0f, 
									0.0f, 0.0f, 1.0f, 0.0f, 
									0.0f, 0.0f, 0.0f, 1.0f };
ZobMatrix4x4::ZobMatrix4x4()
{
	Identity();
}

ZobMatrix4x4::ZobMatrix4x4(const ZobMatrix4x4* m)
{
	CopyFrom(m);
}

ZobMatrix4x4::~ZobMatrix4x4()
{
}

void ZobMatrix4x4::Mul(const ZobMatrix4x4* m)
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

ZobMatrix4x4 ZobMatrix4x4::Transpose() const
{
	ZobMatrix4x4 outM;
	for (int n = 0; n < 4; n++) 
	{
		for (int p = 0; p < 4; p++) 
		{
			outM.SetData(n, p, m_data[p][n]);
		}
	}
	return outM;
}

void ZobMatrix4x4::Identity()
{
	memcpy(&m_data, &identityArray, sizeof(identityArray));
}

void ZobMatrix4x4::CopyFrom(const ZobMatrix4x4* m)
{
	memcpy(&m_data, &m->m_data, sizeof(m_data));
}

void ZobMatrix4x4::AddScale(const ZobVector3& v)
{
	//m_data[0][0] *= v.x;
	//m_data[1][1] *= v.y;
	//m_data[2][2] *= v.z;
	tmp.Identity();
	tmp.m_data[0][0] = v.x;
	tmp.m_data[1][1] = v.y;
	tmp.m_data[2][2] = v.z;
	Mul(&tmp);
}

void ZobMatrix4x4::SetScale(const ZobVector3& v)
{
	tmp.Identity();
	tmp.m_data[0][0] = v.x;
	tmp.m_data[1][1] = v.y;
	tmp.m_data[2][2] = v.z;
	Mul(&tmp);
}

void ZobMatrix4x4::SetScale(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][0] = x;
	tmp.m_data[1][1] = y;
	tmp.m_data[2][2] = z;
	Mul(&tmp);
}

void ZobMatrix4x4::SetPosition(const ZobVector3& v)
{
	//tmp.Identity();
	tmp.m_data[0][3] = v.x;
	tmp.m_data[1][3] = v.y;
	tmp.m_data[2][3] = v.z;
	Mul(&tmp);
}

void ZobMatrix4x4::AddRotation(const ZobVector3& v)
{
	ZobVector3 r = GetRotation();
	SetRotationX(r.x + v.x);
	SetRotationY(r.y + v.y);
	SetRotationZ(r.z + v.z);
}


void ZobMatrix4x4::AddTranslation(const ZobVector3& v)
{
	m_data[0][3] += v.x;
	m_data[1][3] += v.y;
	m_data[2][3] += v.z;
}

void ZobMatrix4x4::SetPosition(const float x, const float y, const float z)
{
	tmp.Identity();
	tmp.m_data[0][3] = x;
	tmp.m_data[1][3] = y;
	tmp.m_data[2][3] = z;
	Mul(&tmp);
}

ZobVector3 ZobMatrix4x4::GetRotation() const
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
	return ZobVector3(heading, bank, attitude);
}

void ZobMatrix4x4::SetRotation(const ZobVector3& v)
{
	float yaw = DEG_TO_RAD(v.z);
	float pitch = DEG_TO_RAD(v.y);
	float roll = DEG_TO_RAD(v.x);
	float cosY = cos(yaw);
	float sinY = sin(yaw);
	float cosP = cos(pitch);
	float sinP = sin(pitch);
	float cosR = cos(roll);
	float sinR = sin(roll);
	m_data[0][0] = cosY * cosP;
	m_data[0][1] = cosY * sinP * sinR - sinY * cosR;
	m_data[0][2] = cosY * sinP * cosR + sinY * sinR;

	m_data[1][0] = sinY * cosP;
	m_data[1][1] = sinY * sinP * sinR + cosY * cosR;
	m_data[1][2] = sinY * sinP * cosR - cosY * sinR;

	m_data[2][0] = -sinP;
	m_data[2][1] = cosP * sinR;
	m_data[2][2] = cosP * cosR;
	//SetRotationX(-v.x);
	//SetRotationY(-v.y);
	//SetRotationZ(-v.z);
}

void ZobMatrix4x4::SetRotation(const float x, const float y, const float z)
{
	ZobVector3 v = ZobVector3(x, y, z);
	SetRotation(v);
	//SetRotationX(-x);
	//SetRotationY(-y);
	//SetRotationZ(-z);
}

void ZobMatrix4x4::SetRotationX(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[1][1] = (float)cos(rx);
	tmp.m_data[1][2] = (float)-sin(rx);
	tmp.m_data[2][1] = (float)sin(rx);
	tmp.m_data[2][2] = (float)cos(rx);
	Mul(&tmp);
}

void ZobMatrix4x4::SetRotationY(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[0][0] = (float)cos(rx);
	tmp.m_data[0][2] = (float)sin(rx);
	tmp.m_data[2][0] = (float)-sin(rx);
	tmp.m_data[2][2] = (float)cos(rx);
	Mul(&tmp);
}

void ZobMatrix4x4::SetRotationZ(const float r)
{
	tmp.Identity();
	double rx = (double)r * M_PI / 180.0;
	tmp.m_data[0][0] = (float)cos(rx);
	tmp.m_data[0][1] = (float)-sin(rx);
	tmp.m_data[1][0] = (float)sin(rx);
	tmp.m_data[1][1] = (float)cos(rx);
	Mul(&tmp);
}

void ZobMatrix4x4::FromVectors(const ZobVector3& left, const ZobVector3& up, const ZobVector3& forward)
{
	SetData(0, 0, left.x);
	SetData(0, 1, up.x);
	SetData(0, 2, forward.x);
	SetData(0, 3, 0);//-p.x);

	SetData(1, 0, left.y);
	SetData(1, 1, up.y);
	SetData(1, 2, forward.y);
	SetData(1, 3, 0);//-p.y);

	SetData(2, 0, left.z);
	SetData(2, 1, up.z);
	SetData(2, 2, forward.z);
	SetData(2, 3, 0);//-p.z);

	SetData(3, 0, 0);
	SetData(3, 1, 0);
	SetData(3, 2, 0);
	SetData(3, 3, 1);
}

ZobVector3 ZobMatrix4x4::EulerToQuaternion(float x, float y, float z)
{
	
	float yaw = z;
	float pitch = y;
	float roll = x;
	// Abbreviations for the various angular functions
	float c1 = cos(x * 0.5f);
	float s1 = sin(x * 0.5f);
	float c2 = cos(y * 0.5f);
	float s2 = sin(y * 0.5f);
	float c3 = cos(z * 0.5f);
	float s3 = sin(z * 0.5f);

	ZobVector3 q;
	q.w = c1 * c2 * c3 - s1 * s2 * s3;
	q.x = s1 * c2 * c3 + c1 * s2 * s3;
	q.y = c1 * s2 * c3 - s1 * c2 * s3;
	q.z = c1 * c2 * s3 + s1 * s2 * c3;
	return q;
	/*
	double roll = x;
	double pitch = y;
	double yaw = z;
	double c1 = cos(roll / 2);
	double s1 = sin(roll / 2);
	double c2 = cos(pitch / 2);
	double s2 = sin(pitch / 2);
	double c3 = cos(yaw / 2);
	double s3 = sin(yaw / 2);
	double c1c2 = c1 * c2;
	double s1s2 = s1 * s2;
	double vw = c1c2 * c3 - s1s2 * s3;
	double vx = c1c2 * s3 + s1s2 * c3;
	double vy = s1 * c2 * c3 + c1 * s2 * s3;
	double vz = c1 * s2 * c3 - s1 * c2 * s3;
	ZobVector3 q;
	q.x = vx;
	q.y = vy;
	q.z = vz;
	q.w = vw;
	return q;
	*/

}

ZobVector3 ZobMatrix4x4::AxisAngleToQuaternion(float angle, const ZobVector3* axis)
{
	double s = sin(angle / 2.0f);
	float x = axis->x * s;
	float y = axis->y * s;
	float z = axis->z * s;
	float w = cos(angle / 2.0f);
	return ZobVector3(x, y, z, w);
}

ZobVector3 ZobMatrix4x4::ToQuaternion()
{
	ZobVector3 q;
	float trace = m_data[0][0] + m_data[1][1] + m_data[2][2]; // I removed + 1.0f; see discussion with Ethan
	if (trace > 0) {// I changed M_EPSILON to 0
		float s = 0.5f / sqrtf(trace + 1.0f);
		q.w = 0.25f / s;
		q.x = (m_data[2][1] - m_data[1][2]) * s;
		q.y = (m_data[0][2] - m_data[2][0]) * s;
		q.z = (m_data[1][0] - m_data[0][1]) * s;
	}
	else {
		if (m_data[0][0] > m_data[1][1] && m_data[0][0] > m_data[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m_data[0][0] - m_data[1][1] - m_data[2][2]);
			q.w = (m_data[2][1] - m_data[1][2]) / s;
			q.x = 0.25f * s;
			q.y = (m_data[0][1] + m_data[1][0]) / s;
			q.z = (m_data[0][2] + m_data[2][0]) / s;
		}
		else if (m_data[1][1] > m_data[2][2]) {
			float s = 2.0f * sqrtf(1.0f + m_data[1][1] - m_data[0][0] - m_data[2][2]);
			q.w = (m_data[0][2] - m_data[2][0]) / s;
			q.x = (m_data[0][1] + m_data[1][0]) / s;
			q.y = 0.25f * s;
			q.z = (m_data[1][2] + m_data[2][1]) / s;
		}
		else {
			float s = 2.0f * sqrtf(1.0f + m_data[2][2] - m_data[0][0] - m_data[1][1]);
			q.w = (m_data[1][0] - m_data[0][1]) / s;
			q.x = (m_data[0][2] + m_data[2][0]) / s;
			q.y = (m_data[1][2] + m_data[2][1]) / s;
			q.z = 0.25f * s;
		}
	}
	return q;
}

ZobVector3 ZobMatrix4x4::QuaternionToEuler(float x, float y, float z, float w)
{

	static int testQ = 1;
	if (testQ == 0)
	{
		double roll, pitch, yaw;
		// roll (x-axis rotation)
		double sinr_cosp = 2 * (w * x + y * z);
		double cosr_cosp = 1 - 2 * (x * x + y * y);
		roll = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2 * (w * y - z * x);
		if (std::abs(sinp) >= 1)
			pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
		else
			pitch = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (w * z + x * y);
		double cosy_cosp = 1 - 2 * (y * y + z * z);
		yaw = std::atan2(siny_cosp, cosy_cosp);
		return ZobVector3((float)roll, (float)pitch, (float)yaw);
	}
	else if (testQ == 1)
	{
		double roll;
		double pitch;
		double yaw;
		double sqw = w * w;
		double sqx = x * x;
		double sqy = y * y;
		double sqz = z * z;
		double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		double test = x* y + z * w;
		if (test > 0.499 * unit) { // singularity at north pole
			roll = 2 * atan2(x, w);
			pitch = M_PI / 2;
			yaw = 0;
		}
		else if (test < -0.499 * unit) { // singularity at south pole
			roll = -2 * atan2(x, w);
			pitch = -M_PI / 2;
			yaw = 0;
		}
		else
		{
			//roll = atan2(2 * y * w - 2 * x * z, sqx - sqy - sqz + sqw);
			//pitch = asin(2 * test / unit);
			//yaw = atan2(2 * x * w - 2 * y * z, -sqx + sqy - sqz + sqw);
			roll = atan2(-2 * (y * z - w * x), w * w - x * x - y * y + z * z);
			float s = 2 * (x * z + w * y);
			s = fminf(fmaxf(s, -1.0f), 1.0f);
			pitch = asin(s);
			yaw = atan2(-2 * (x * y - w * z), w * w + x * x - y * y - z * z);
		}
		return ZobVector3(roll, pitch, yaw);
	}
	else
	{
		double roll;
		double pitch;
		double yaw;
		double test = x * y + z * w;
		if (test > 0.499) { // singularity at north pole
			roll = 2 * atan2(x, w);
			pitch = M_PI / 2;
			yaw = 0;
		}
		if (test < -0.499) { // singularity at south pole
			roll = -2 * atan2(x, w);
			pitch = -M_PI / 2;
			yaw = 0;
		}
		else
		{
			double sqx = x * x;
			double sqy = y * y;
			double sqz = z * z;
			roll = atan2(2 * y * w - 2 * x * z, 1 - 2 * sqy - 2 * sqz);
			pitch = asin(2 * test);
			yaw = atan2(2 * x * w - 2 * y * z, 1 - 2 * sqx - 2 * sqz);
		}
		return ZobVector3(roll, pitch, yaw);
	}
}

void ZobMatrix4x4::InvertMatrix4(const ZobMatrix4x4& m, ZobMatrix4x4& im)
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

ZobMatrix4x4 ZobMatrix4x4::RotateAroundAxis(const ZobVector3& axis, const float angle)
{
	ZobMatrix4x4 rotationMatrix;
	float u = axis.x;
	float v = axis.y;
	float w = axis.z;
	float L = (u * u + v * v + w * w);
	float a = angle * M_PI / 180.0; //converting to radian value
	float u2 = u * u;
	float v2 = v * v;
	float w2 = w * w;
	float cosa = cos(a);
	float sina = sin(a);
	float sqrtL = sqrt(L);
	rotationMatrix.m_data[0][1] = (u * v * (1 - cosa) - w * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[0][2] = (u * w * (1 - cosa) + v * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[0][3] = 0.0;
				  
	rotationMatrix.m_data[1][0] = (u * v * (1 - cosa) + w * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[0][0] = (u2 + (v2 + w2) * cosa) / L;
	rotationMatrix.m_data[1][1] = (v2 + (u2 + w2) * cosa) / L;
	rotationMatrix.m_data[1][2] = (v * w * (1 - cosa) - u * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[1][3] = 0.0;
				 
	rotationMatrix.m_data[2][0] = (u * w * (1 - cosa) - v * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[2][1] = (v * w * (1 - cosa) + u * sqrtL * sin(a)) / L;
	rotationMatrix.m_data[2][2] = (w2 + (u2 + v2) * cosa) / L;
	rotationMatrix.m_data[2][3] = 0.0;
				
	rotationMatrix.m_data[3][0] = 0.0;
	rotationMatrix.m_data[3][1] = 0.0;
	rotationMatrix.m_data[3][2] = 0.0;
	rotationMatrix.m_data[3][3] = 1.0;
	return rotationMatrix;
}

const ZobMatrix4x4 ZobMatrix4x4::IdentityMatrix = ZobMatrix4x4();