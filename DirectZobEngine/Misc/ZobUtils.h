#pragma once
#include <string>

class ZobUtils
{
public:
	static 	std::string floatToString(float f, int precision)
	{
		return std::to_string(f).substr(0, std::to_string(f).find(".") + precision + 1);
	};

	static std::string GetTempDirectory()
	{
#ifdef LINUX
		return std::string("//tmp//");
#elif WINDOWS
		char lpTempPathBuffer[MAX_PATH];
		GetTempPath(MAX_PATH, lpTempPathBuffer); 
		return std::string(lpTempPathBuffer);
#elif MACOS
		return std::string(NSTemporaryDirectory();
#endif		
	}
};