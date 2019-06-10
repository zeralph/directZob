#include "Vector2.h"
#include <math.h>

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

void Vector2::Norm()
{
	float l = sqrtLenght();
	x /= l;
	y /= l;
}

float Vector2::Dot(const Vector2 * v)
{
	return x * v->x + y * v->y;
}

float Vector2::lenght()
{
	return (x*x + y * y);
}

float Vector2::sqrtLenght()
{
	return (float)sqrt((double)x * (double)x + (double)y * (double)y);
}

const Vector2 Vector2::Vector2Zero = Vector2(0.0f, 0.0f);
const Vector2 Vector2::Vector2One = Vector2(1.0f, 1.0f);
const Vector2 Vector2::Vector2X = Vector2(1.0f, 0.0f);
const Vector2 Vector2::Vector2Y = Vector2(0.0f, 1.0f);