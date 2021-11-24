#include "ZobVector3.h"
#include "Misc/ZobXmlHelper.h"

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

bool ZobVector3::FromString(std::string& sx, std::string& sy, std::string& sz)
{
	x = atof(sx.c_str());
	y = atof(sy.c_str());
	z = atof(sz.c_str());
	return true;
}

bool ZobVector3::FromString(std::string &s)
{
	std::size_t del1, del2= 0;
	del1 = s.find(';');
	if (del1 != std::string::npos)
	{
		del2 = s.find(';', del1 + 1); 
		if (del1 != std::string::npos)
		{
			x = atof(s.substr(0, del1).c_str());
			y = atof(s.substr(del1+1, del2).c_str());
			z = atof(s.substr(del2+1, s.size()-1).c_str());
			return true;
		}
	}
	return false;
}

const ZobVector3 ZobVector3::Vector3Zero = ZobVector3(0.0f, 0.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3One = ZobVector3(1.0f, 1.0f, 1.0f);
const ZobVector3 ZobVector3::Vector3X = ZobVector3(1.0f, 0.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3Y = ZobVector3(0.0f, 1.0f, 0.0f);
const ZobVector3 ZobVector3::Vector3Z = ZobVector3(0.0f, 0.0f, 1.0f);