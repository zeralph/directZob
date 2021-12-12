#pragma once
#include <string>
#include "../Misc/ZobUtils.h"

class ZobVector2
{
public:
	ZobVector2();
	ZobVector2(float x, float y);
	ZobVector2(const ZobVector2* v)
	{
		x = v->x;
		y = v->y;
		w = v->w;
	}

	static const ZobVector2 Vector2Zero;
	static const ZobVector2 Vector2One;
	static const ZobVector2 Vector2X;
	static const ZobVector2 Vector2Y;

	bool operator== (const ZobVector2& v2) { return x == v2.x && y == v2.y && w == v2.w; }
	
	inline void Copy(const ZobVector2* v)
	{
		x = v->x;
		y = v->y;
		w = v->w;
	}
	inline void Mul(float f)
	{
		x *= f;
		y *= f;
	}
	bool FromString(std::string& s);
	std::string ToString() 
	{ 
		return ZobUtils::floatToString(x, 3).append(";").append(ZobUtils::floatToString(y, 3));
		//return std::to_string(x).append(";") + std::to_string(y).append(";"); 
	}
	void Norm();
	float Dot(const ZobVector2 * v);
	float Lenght();
	float SqrtLenght();

public:
	float x;
	float y;
	float w;
private:

};

static ZobVector2 operator+ (const ZobVector2& v1, const ZobVector2& v2) { return ZobVector2(v1.x + v2.x, v1.y + v2.y); }
