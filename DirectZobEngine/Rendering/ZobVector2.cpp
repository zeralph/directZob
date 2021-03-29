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
	float l = SqrtLenght();
	x /= l;
	y /= l;
}

float ZobVector2::Dot(const ZobVector2 * v)
{
	return x * v->x + y * v->y;
}

float ZobVector2::Lenght()
{
	return (x*x + y * y);
}

float ZobVector2::SqrtLenght()
{
	return (float)sqrt((double)x * (double)x + (double)y * (double)y);
}

bool ZobVector2::FromString(std::string& s)
{
	std::size_t del1, del2 = 0;
	del1 = s.find(';');
	if (del1 != std::string::npos)
	{
		del2 = s.find(';', del1 + 1);
		if (del1 != std::string::npos)
		{
			x = atof(s.substr(0, del1).c_str());
			y = atof(s.substr(del1 + 1, del2).c_str());
			return true;
		}
	}
	return false;
}

const ZobVector2 ZobVector2::Vector2Zero = ZobVector2(0.0f, 0.0f);
const ZobVector2 ZobVector2::Vector2One = ZobVector2(1.0f, 1.0f);
const ZobVector2 ZobVector2::Vector2X = ZobVector2(1.0f, 0.0f);
const ZobVector2 ZobVector2::Vector2Y = ZobVector2(0.0f, 1.0f);