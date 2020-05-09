#pragma once
#include "Vector3.h"
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
		enum Lighting_precision
		{
			Lighting_precision_noLighting = 0,
			Lighting_precision_vertex,
			Lighting_precision_pixel,
			__Lighting_precision_MAX__
		};
		inline const bool ZBuffered() const { return zBuffered; }
		void ZBuffered(const bool b)
		{
			zBuffered = b;
		}
		inline const eLightMode LightMode() const 
		{ 
			return lightMode;
		}
		void LightMode(eLightMode l)
		{
			lightMode = l;
		}
		void Save()
		{

		}
		bool bTransparency = false;
		bool bColorize = false;
		Vector3 colorization = Vector3();
		bool zBuffered = true;
		eLightMode lightMode = eLightMode::eLightMode_phong;
	};
};

enum FogType
{
	FogType_NoFog = 0,
	FogType_Linear,
	FogType_Exp,
	FogType_Exp2,
	__FogType_MAX__
};
typedef DirectZobType::u8 u8;
typedef DirectZobType::uint uint;
typedef DirectZobType::ulong ulong;
typedef unsigned long long zobId;
typedef struct DirectZobType::BufferData BufferData;
typedef struct DirectZobType::Line3D Line3D;
typedef struct DirectZobType::RenderOptions RenderOptions;

static Vector3 Vector2Color(Vector3* v) { return Vector3((int)(v->x * 255.0f), (int)(v->y * 255.0f), (int)(v->z * 255.0f)); };
static Vector3 Color2Vector(Vector3* v) { return Vector3(v->x / 255.0f, v->y / 255.0f, v->z / 255.0f); };