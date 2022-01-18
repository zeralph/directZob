#include "ZobVector3.h"
#include "Misc/ZobXmlHelper.h"
#include <sstream>

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
	w = v->w;
}

bool ZobVector3::FromString(std::string& sx, std::string& sy, std::string& sz, std::string& sw)
{
	x = atof(sx.c_str());
	y = atof(sy.c_str());
	z = atof(sz.c_str());
	w = atof(sw.c_str());
	return true;
}

std::string ZobVector3::ToString()
{
	int dec = 5;
	std::stringstream ss; 
	ss << ZobUtils::floatToString(x, dec) << ";" << ZobUtils::floatToString(y, dec) << ";" << ZobUtils::floatToString(z, dec) << ";" << ZobUtils::floatToString(w, dec);
	return ss.str();
}

bool ZobVector3::FromString(std::string &s)
{
	std::size_t del1, del2, del3= 0;
	del1 = s.find(';');
	if (del1 != std::string::npos)
	{
		del2 = s.find(';', del1 + 1); 
		if (del2 != std::string::npos)
		{
			del3 = s.find(';', del2 + 1);
			if (del3 != std::string::npos)
			{
				x = atof(s.substr(0, del1).c_str());
				y = atof(s.substr(del1 + 1, del2).c_str());
				z = atof(s.substr(del2 + 1, del3).c_str());		
				w = atof(s.substr(del3 + 1, s.size() - 1).c_str());
			}
			else
			{
				x = atof(s.substr(0, del1).c_str());
				y = atof(s.substr(del1 + 1, del2).c_str());
				z = atof(s.substr(del2 + 1, del3).c_str());
				w = 1.0f;
			}
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