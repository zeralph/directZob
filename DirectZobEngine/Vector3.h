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

	inline void Copy(const Vector3* v)
	{
		x = v->x;
		y = v->y;
		z = v->z;
		w = v->w;
	}

	inline void Normalize() 
	{
		float f = sqrtLength();
		x /= f; 
		y /= f; 
		z /= f;
		w = 1.0f;
	}

	inline float sqrtLength() { return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z); }

	static inline float Dot(const Vector3* v1, const Vector3* v2) { return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z; };
	static inline Vector3 Cross(const Vector3* v1, const Vector3* v2)
	{
		Vector3 r;
		r.x = v1->y * v2->z - v1->z * v2->y;
		r.y = v1->z * v2->x - v1->x * v2->z;
		r.z = v1->x * v2->y - v1->y * v2->x;
		//r.z = v1->x * v2->y - v1->y * v2->x;
		return r;
	};
	static Vector3 RotateAroundAxis(const Vector3& v, const Vector3& axe, float angle);
	inline void Add(const Vector3* v) { x += v->x; y += v->y; z += v->z; }
	inline void Div(float f) { x /= f; y /= f; z /= f; }
	inline void Mul(float f) { x *= f; y *= f; z *= f; }
	inline void Set(const Vector3* v) { x = v->x; y = v->y; z = v->z; }
	inline bool operator== (const Vector3& v) { return (x == v.x && y == v.y && v.z == z && w == v.w); }
	inline bool operator!= (const Vector3& v) { return (x != v.x || y != v.y || v.z != z || w != v.w); }
	inline void operator/= (const float f) { x /= f; y /= f; z /= f; }
	inline void operator*= (const float f) { x *= f; y *= f; z *= f; }
//	inline Vector3 operator= (const Vector3 &v) { return Vector3(v.x, v.y, v.z); }

public:
	float x;
	float y;
	float z;
	float w;
};

static inline Vector3 operator+ (const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
static inline Vector3 operator- (const Vector3& v1, const Vector3& v2) { return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
static inline Vector3 operator* (const Vector3& v, const float f) { return Vector3(v.x *f, v.y *f, v.z *f); }
static inline Vector3 operator/ (const Vector3& v, const float f) { return Vector3(v.x / f, v.y / f, v.z / f); }