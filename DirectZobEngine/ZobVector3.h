#pragma once
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
class ZobVector3
{
public:
	ZobVector3();
	ZobVector3(float x, float y, float z);
	ZobVector3(const ZobVector3* v);

	static const ZobVector3 Vector3Zero;
	static const ZobVector3 Vector3One;
	static const ZobVector3 Vector3X;
	static const ZobVector3 Vector3Y;
	static const ZobVector3 Vector3Z;

	inline void Copy(const ZobVector3* v)
	{
		x = v->x;
		y = v->y;
		z = v->z;
		w = v->w;
	}

	inline void Normalize() 
	{
		float f = sqrtLength();
		if (f != 0.0f)
		{
			x /= f;
			y /= f;
			z /= f;
			w = 1.0f;
		}
	}

	inline float sqrtLength() { return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z); }

	static inline float Dot(const ZobVector3* v1, const ZobVector3* v2) { return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z; };
	static inline ZobVector3 Cross(const ZobVector3* v1, const ZobVector3* v2)
	{
		ZobVector3 r;
		r.x = v1->y * v2->z - v1->z * v2->y;
		r.y = v1->z * v2->x - v1->x * v2->z;
		r.z = v1->x * v2->y - v1->y * v2->x;
		//r.z = v1->x * v2->y - v1->y * v2->x;
		return r;
	};
	static inline ZobVector3 GetAnglesFromVector(ZobVector3 v)
	{
		
		
		ZobVector3 vOut = v;
		vOut.Normalize();
		float pitch = asin(vOut.y);
		float yaw = atan2(vOut.x, vOut.z);
		pitch = atan2(vOut.x, vOut.y);
		yaw = atan2(vOut.z, sqrt((vOut.x * vOut.x) + (vOut.y * vOut.y)));
		vOut.x = pitch * 180.0f / M_PI;
		vOut.y = yaw * 180.0f / M_PI;
		vOut.z = 0.0f * vOut.z;
		return vOut;
		
		//float yaw = atan2(ds.X, ds.Y); 
		//float pitch = Math.Atan2(ds.Z, Math.Sqrt((ds.X * ds.X) + (ds.Y * ds.Y)));

	};
	inline void Add(const ZobVector3* v) { x += v->x; y += v->y; z += v->z; }
	inline void Div(float f) { x /= f; y /= f; z /= f; }
	inline void Mul(float f) { x *= f; y *= f; z *= f; }
	inline void Set(const ZobVector3* v) { x = v->x; y = v->y; z = v->z; }
	inline const bool operator== (const ZobVector3& v) { return (x == v.x && y == v.y && v.z == z && w == v.w); }
	inline const bool operator== (const ZobVector3* v) { return (x == v->x && y == v->y && v->z == z && w == v->w); }
	inline const bool operator!= (const ZobVector3& v) { return (x != v.x || y != v.y || v.z != z || w != v.w); }
	inline const bool operator!= (const ZobVector3* v) { return (x != v->x || y != v->y || v->z != z || w != v->w); }
	inline void operator/= (const float f) { x /= f; y /= f; z /= f; }
	inline void operator*= (const float f) { x *= f; y *= f; z *= f; }
//	inline ZobVector3 operator= (const ZobVector3 &v) { return ZobVector3(v.x, v.y, v.z); }

public:
	float x;
	float y;
	float z;
	float w;
};

static inline ZobVector3 operator+ (const ZobVector3& v1, const ZobVector3& v2) { return ZobVector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
static inline ZobVector3 operator- (const ZobVector3& v1, const ZobVector3& v2) { return ZobVector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
static inline ZobVector3 operator* (const ZobVector3& v, const float f) { return ZobVector3(v.x *f, v.y *f, v.z *f); }
static inline ZobVector3 operator* (const ZobVector3& u, const ZobVector3& v) { return ZobVector3(v.x * u.x, v.y * u.y, v.z * u.z); }
static inline ZobVector3 operator/ (const ZobVector3& v, const float f) { return ZobVector3(v.x / f, v.y / f, v.z / f); }
static inline bool operator!= (const ZobVector3& v1, const ZobVector3& v2) { return (v1.x != v2.x || v1.y != v2.y || v2.z != v1.z || v1.w != v2.w); }
static inline bool operator!= (const ZobVector3* v1, const ZobVector3& v2) { return (v1->x != v2.x || v1->y != v2.y || v2.z != v1->z || v1->w != v2.w); }
static inline bool operator!= (const ZobVector3& v1, const ZobVector3* v2) { return (v2->x != v1.x || v2->y != v1.y || v1.z != v2->z || v2->w != v1.w); }