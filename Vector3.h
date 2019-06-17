#pragma once
#include <math.h>
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);

	static const Vector3 Vector3Zero;
	static const Vector3 Vector3One;
	static const Vector3 Vector3X;
	static const Vector3 Vector3Y;
	static const Vector3 Vector3Z;

	inline void Normalize() 
	{
		float f = (float)sqrt((float)(x * x) + (float)(y * y) + (float)(z * z));
		x /= f; 
		y /= f; 
		z /= f;
		w = 1.0f;
	}

	static float Dot(const Vector3* v1, const Vector3* v2);
	static Vector3 Cross(const Vector3* v1, const Vector3* v2);

	bool operator== (const Vector3& v) { return (x == v.x && y == v.y && v.z == z && w == v.w); }

public:
	float x;
	float y;
	float z;
	float w;
};

static Vector3 operator+ (const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
//static Vector3 operator+ (const Vector3* v1, const Vector3* v2) { return Vector3(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z); }