#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <random>
#ifdef WINDOWS
#include <windows.h>
#include <algorithm>
#endif
class ZobUtils
{
public:


	template<typename T>
	static T random(T range_from, T range_to) {
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<T>    distr(range_from, range_to);
		return distr(generator);
	}

	static 	std::string floatToString(float f, int precision)
	{
		return std::to_string(f).substr(0, std::to_string(f).find(".") + precision + 1);
	};

	static std::string GetTempFile(std::string extension)
	{
		char* t = NULL;
#ifdef LINUX
		t = tempnam(NULL, "dzob");
#elif WINDOWS
		t = _tempnam(NULL, "dzob");
#elif MACOS
		mkstemp(t);
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
		GetTempPathA(MAX_PATH, lpTempPathBuffer);
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