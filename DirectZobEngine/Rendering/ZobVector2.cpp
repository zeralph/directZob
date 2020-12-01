#include "ZobVector2.h"
#include <math.h>

ZobVector2::ZobVector2() :
	x(0.0f),
	y(0.0f),
	w(1.0f)
{

}

ZobVector2::ZobVector2(float x, float y) :
	x(x),
	y(y),
	w(1.0f)
{

}

void ZobVector2::Norm()
{
	float l = sqrtLenght();
	x /= l;
	y /= l;
}

float ZobVector2::Dot(const ZobVector2 * v)
{
	return x * v->x + y * v->y;
}

float ZobVector2::lenght()
{
	return (x*x + y * y);
}

float ZobVector2::sqrtLenght()
{
	return (float)sqrt((double)x * (double)x + (double)y * (double)y);
}

const ZobVector2 ZobVector2::Vector2Zero = ZobVector2(0.0f, 0.0f);
const ZobVector2 ZobVector2::Vector2One = ZobVector2(1.0f, 1.0f);
const ZobVector2 ZobVector2::Vector2X = ZobVector2(1.0f, 0.0f);
const ZobVector2 ZobVector2::Vector2Y = ZobVector2(0.0f, 1.0f);