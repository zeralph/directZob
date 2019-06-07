#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);

	static const Vector2 Vector2Zero;
	static const Vector2 Vector2One;
	static const Vector2 Vector2X;
	static const Vector2 Vector2Y;

	bool operator== (const Vector2& v2) { return x == v2.x && y == v2.y && w == v2.w; }

public:
	float x;
	float y;
	float w;
};

