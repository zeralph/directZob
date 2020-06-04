#include "ZobVector3.h"

ZobVector3::ZobVector3() :
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(1.0f)
{

}

ZobVector3::ZobVector3(float x, float y, float z) :
	x(x),
	y(y),
	z(z),
	w(1.0f)
{

}

ZobVector3::ZobVector3(const ZobVector3* v)
{
	x = v->x;
	y = v->y;
	z = v->z;
	w = v-> w;
}

const ZobVector3 ZobVector3::Vector3Zero = ZobVector3(0.0f, 0.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3One = ZobVector3(1.0f, 1.0f, 1.0f);
const ZobVector3 ZobVector3::Vector3X = ZobVector3(1.0f, 0.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3Y = ZobVector3(0.0f, 1.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3Z = ZobVector3(0.0f, 0.0f, 1.0f);