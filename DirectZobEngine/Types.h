#pragma once
#include <stddef.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



class __declspec(dllexport) DirectZobType
{
public:
	typedef char u8;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	struct BufferData
	{
		uint width;
		uint height;
		uint size;
		float zNear;
		float zFar;
		uint* buffer;
		float* zBuffer;
	};

	struct Line2D
	{
		float xa;
		float xb;
		float ya;
		float yb;
		float c;
	};
};

typedef DirectZobType::u8 u8;
typedef DirectZobType::uint uint;
typedef DirectZobType::ulong ulong;
typedef struct DirectZobType::BufferData BufferData;
typedef struct DirectZobType::Line2D Line2D;