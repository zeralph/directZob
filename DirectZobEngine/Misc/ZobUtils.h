#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#ifdef WINDOWS
#include <windows.h>
#include <algorithm>
#endif
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
		//std::filesystem::temp_directory_path()
		char lpTempPathBuffer[MAX_PATH];
		GetTempPath(MAX_PATH, lpTempPathBuffer);
		std::string s = std::string(lpTempPathBuffer);
		std::replace(s.begin(), s.end(), '\\', '/');
		return s;
#elif MACOS
		return std::string(NSTemporaryDirectory();
#endif		
	}
};