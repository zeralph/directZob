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

	static std::string GetTempFile(std::string extension)
	{
#ifdef LINUX
		char* t = _tempnam(NULL, "dzob");
#elif WINDOWS
		char* t = tempnam(NULL, "dzob");
#elif MACOS
		char* t = tempnam(NULL, "dzob");
#endif	
		return std::string(t).append(extension);
	}

	static std::string GetTempDirectory()
	{
		//char* t = _tempnam(NULL, "pref");
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
		//char* t = tmpnam(NULL);
		//return std::string(t);
		return std::string("//var//tmp//");
#endif		
	}
};