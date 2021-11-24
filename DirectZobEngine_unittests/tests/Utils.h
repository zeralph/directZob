#pragma once
#include <Windows.h>


class MyUtils
{
public :
	static std::string GetPath()
	{
		wchar_t wbuffer[MAX_PATH];
		char buffer[MAX_PATH];
		GetModuleFileName(NULL, wbuffer, MAX_PATH);
		sprintf(buffer, "%ls", wbuffer);
		std::string p = std::string(buffer);
		size_t i = p.find("DirectZobEngine_unittests");
		return p.substr(0, i);
	}
};