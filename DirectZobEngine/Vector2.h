#ifndef DZOB_VECTOR2_H
#define DZOB_VECTOR2_H
namespace directZob {
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
	
	inline void Copy(Vector2* v)
	{
		x = v->x;
		y = v->y;
		w = v->w;
	}

	void Norm();
	float Dot(const Vector2 * v);
	float lenght();
	float sqrtLenght();

public:
	float x;
	float y;
	float w;
};

static Vector2 operator+ (const Vector2& v1, const Vector2& v2) { return Vector2(v1.x + v2.x, v1.y + v2.y); }
}
#endif