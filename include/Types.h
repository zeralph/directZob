#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

