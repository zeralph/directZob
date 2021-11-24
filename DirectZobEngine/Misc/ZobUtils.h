#pragma once
#include <string>

class ZobUtils
{
public:
	static 	std::string floatToString(float f, int precision)
	{
		return std::to_string(f).substr(0, std::to_string(f).find(".") + precision + 1);
	};
};