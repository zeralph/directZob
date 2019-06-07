#include "Vector2.h"


Vector2::Vector2() :
	x(0.0f),
	y(0.0f),
	w(1.0f)
{

}

Vector2::Vector2(float x, float y) :
	x(x),
	y(y),
	w(1.0f)
{

}

const Vector2 Vector2::Vector2Zero = Vector2(0.0f, 0.0f);
const Vector2 Vector2::Vector2One = Vector2(1.0f, 1.0f);
const Vector2 Vector2::Vector2X = Vector2(1.0f, 0.0f);
const Vector2 Vector2::Vector2Y = Vector2(0.0f, 1.0f);