#pragma once
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
	
	inline void Copy(ZobVector2* v)
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
	void Norm();
	float Dot(const ZobVector2 * v);
	float lenght();
	float sqrtLenght();

public:
	float x;
	float y;
	float w;
};

static ZobVector2 operator+ (const ZobVector2& v1, const ZobVector2& v2) { return ZobVector2(v1.x + v2.x, v1.y + v2.y); }
