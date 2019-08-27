#pragma once
#include <math.h>
class Vector3
{
public:
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const Vector3* v);

	static const Vector3 Vector3Zero;
	static const Vector3 Vector3One;
	static const Vector3 Vector3X;
	static const Vector3 Vector3Y;
	static const Vector3 Vector3Z;

	inline void Normalize() 
	{
		float f = sqrtLength();
		x /= f; 
		y /= f; 
		z /= f;
		w = 1.0f;
	}

	inline float sqrtLength() { return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z); }

	static float Dot(const Vector3* v1, const Vector3* v2);
	static Vector3 Cross(const Vector3* v1, const Vector3* v2);
	static Vector3 RotateAroundAxis(const Vector3& v, const Vector3& axe, float angle);
	inline void Add(const Vector3* v) { x += v->x; y += v->y; z += v->z; }
	inline void Div(float f) { x /= f; y /= f; z /= f; }
	inline void Set(const Vector3* v) { x = v->x; y = v->y; z = v->z; }
	bool operator== (const Vector3& v) { return (x == v.x && y == v.y && v.z == z && w == v.w); }
//	Vector3 operator= (const Vector3 &v) { x = v.x; y=v.y, z=v.z; }

public:
	float x;
	float y;
	float z;
	float w;
};

static Vector3 operator+ (const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
static Vector3 operator- (const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
static Vector3 operator* (const Vector3& v, const float f) { return Vector3(v.x *f, v.y *f, v.z *f); }
