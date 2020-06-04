
#pragma once
#ifdef __clang__
	#pragma clang diagnostic ignored "-Wpragma-pack"
#endif
#include "ZobVector3.h"
#include <stddef.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifdef LINUX
    #define MAX_PATH 256
    #include <unistd.h>
	#define SLEEP(a) sleep(a/1000)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
    #define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#elif WINDOWS
	#include <windows.h>
	#define SLEEP(a) Sleep(a)
#elif MACOS
    #define MAX_PATH 256
    #include <unistd.h>
	#define SLEEP(a) sleep(a/1000)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#endif //LINUX

enum eRenderOutput
{
	eRenderOutput_render = 0,
	eRenderOutput_zBuffer,
	eRenderOutput_oBuffer,
	__eRenderOutput_MAX__
};
enum eRenderMode
{
	eRenderMode_fullframe = 0,
	eRenderMode_interlaced,
	eRenderMode_scanline,
	__eRenderMode_MAX__
};
enum eCullMode
{
	eCullMode_None = 0,
	eCullMode_ClockwiseFace,
	eCullMode_CounterClockwiseFace,
	__eCullMode_MAX__,
};
enum eLightingPrecision
{
	eLightingPrecision_noLighting = 0,
	eLightingPrecision_vertex,
	eLightingPrecision_pixel,
	__eLightingPrecision_MAX__
};
enum eFogType
{
	eFogType_NoFog = 0,
	eFogType_Linear,
	eFogType_Exp,
	eFogType_Exp2,
	__eFogType_MAX__
};

class  DirectZobType
{
public:
	
	typedef char u8;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef unsigned long long int guid; 
	struct BufferData
	{
		uint width;
		uint height;
		uint size;
		float zNear;
		float zFar;
		uint* buffer;
		float* zBuffer;
//		uint* oBuffer;
	};

	struct Line3D
	{
		float xa;
		float xb;
		float ya;
		float yb;
		float za;
		float zb;
		float c;
		bool bold = false;
	};
	struct RenderOptions
	{
		enum eLightMode
		{
			eLightMode_none = 0,
			eLightMode_flat,
			eLightMode_gouraud,
			eLightMode_phong,
			eLightMode_flatPhong,
			__eLightMode_MAX__
		};
		bool bTransparency = false;
		bool bColorize = false;
		ZobVector3 colorization = ZobVector3();
		bool zBuffered = true;
		eLightMode lightMode = eLightMode::eLightMode_phong;
		eCullMode cullMode = eCullMode_ClockwiseFace;
	};
};

typedef DirectZobType::u8 u8;
typedef DirectZobType::uint uint;
typedef DirectZobType::ulong ulong;
typedef unsigned long long zobId;
typedef struct DirectZobType::BufferData BufferData;
typedef struct DirectZobType::Line3D Line3D;
typedef struct DirectZobType::RenderOptions RenderOptions;

static ZobVector3 Vector2Color(ZobVector3* v) { return ZobVector3((int)(v->x * 255.0f), (int)(v->y * 255.0f), (int)(v->z * 255.0f)); };
static ZobVector3 Color2Vector(ZobVector3* v) { return ZobVector3(v->x / 255.0f, v->y / 255.0f, v->z / 255.0f); };