#include "Vector3.h"


Vector3::Vector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{

}

Vector3::Vector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z),
	w(1.0f)
{

}

Vector3::Vector3(const Vector3* v)
{
	x = v->x;
	y = v->y;
	z = v->z;
	w = v-> w;
}

Vector3 Vector3::RotateAroundAxis(const Vector3& p, const Vector3& axe, float angle)
{
	float u = axe.x;
	float v = axe.y;
	float w = axe.z;
	float x = p.x;
	float y = p.y;
	float z = p.z;
	float c = cos(angle);
	float s = sin(angle);

	float ox = u * (u*x + v * y + w * z)*(1 - c) + x * c + (-w * y + v * z)*s;
	float oy = v * (u*x + v * y + w * z)*(1 - c) + y * c + (w*x - u * z)*s;
	float oz = w * (u*x + v * y + w * z)*(1 - c) + z * c + (-v * x + u * y)*s;

	return Vector3(ox, oy, oz);
}

Vector3 Vector3::Cross(const Vector3* v1, const Vector3* v2)
{
	Vector3 r;
	r.x = v1->y * v2->z - v1->z * v2->y;
	r.y = v1->z * v2->x - v1->x * v2->z;
	r.z = v1->x * v2->y - v1->y * v2->x;
	//r.z = v1->x * v2->y - v1->y * v2->x;
	return r;
}

float Vector3::Dot(const Vector3* v1, const Vector3* v2)
{
	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

const Vector3 Vector3::Vector3Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::Vector3One = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Vector3X = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Vector3Y = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Vector3Z = Vector3(0.0f, 0.0f, 1.0f);