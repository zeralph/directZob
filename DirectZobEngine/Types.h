
#pragma once
#ifdef __clang__
	#pragma clang diagnostic ignored "-Wpragma-pack"
#endif
#include "Rendering/ZobVector3.h"
#include <stddef.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifdef LINUX
    #define MAX_PATH 256
    #include <unistd.h>
	#define _TRUNCATE ((size_t)-1)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define _vscprintf(a,...) snprintf(NULL,0,a,__VA_ARGS__)
    #define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#elif WINDOWS
	#include <windows.h>
#elif MACOS
    #define MAX_PATH 256
    #include <unistd.h>
	#define _TRUNCATE ((size_t)-1)
	#define _snprintf_s(a,b,c,...) snprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define _vsnprintf_s(a,b,c,...) vsnprintf(a,b,c,__VA_ARGS__)
	#define _vscprintf(a,...) snprintf(NULL,0,a,__VA_ARGS__)
	#define fopen_s(fp, fmt, mode)  ({\
        *(fp)=fopen( (fmt), (mode));\
        (*(fp) ) ? 0:errno;\
    })
#endif //LINUX
#ifndef MIN
	#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
	#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef CLAMP
	#define CLAMP( x, xmin, xmax )    MAX( (xmin), MIN( (xmax), (x) ) )
#endif
#ifndef RAD_TO_DEG
	#define RAD_TO_DEG(x) (x * 180.0f / M_PI)
#endif
#ifndef DEG_TO_RAD
	#define DEG_TO_RAD(x) (x * M_PI / 180.0f)
#endif
enum eRenderMode
{
	eRenderMode_fullframe = 0,
	eRenderMode_interlaced,
	eRenderMode_scanline,
	__eRenderMode_MAX__
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
enum eBitsPerColor
{
	eBitsPerColor_full = 0,
	eBitsPerColor_1 = 1,
	eBitsPerColor_2 = 2,
	eBitsPerColor_3 = 3,
	eBitsPerColor_4 = 4,
	eBitsPerColor_5 = 5,
	eBitsPerColor_6 = 6,
	eBitsPerColor_7 = 7,
	eBitsPerColor_8 = 8,
	__eBitsPerColor_MAX__
};
class  DirectZobType
{
public:
	
	typedef char u8;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef unsigned short int u16;
	typedef unsigned int guid;
	typedef unsigned long long zobId;
	static std::string floatToString(float f, int precision)
	{
		return std::to_string(f).substr(0, std::to_string(f).find(".") + precision + 1);
	};
	struct BufferData
	{
		uint width;
		uint height;
		uint size;
		float zNear;
		float zFar;
		uint* buffer;
		float* zBuffer;
		int curBuffer;
	};
	struct ZobFilePath
	{
		std::string name;
		std::string path;
		std::string file;
		bool bAbsolute;
		void Init()
		{
			name = "";
			path = "";
			file = "";
			bAbsolute = false;
		}
		bool IsDefined()
		{
			return name.size() && path.size() && file.size();
		}
		std::string GetFullPath()
		{
			return "";
		}
		std::string Serialize()
		{
			std::string s = name;
			s = s.append(";").append(path).append(";").append(file);
			return s;
		}
		void Unserialize(std::string s)
		{
			std::size_t del1, del2 = 0;
			del1 = s.find(';');
			if (del1 != std::string::npos)
			{
				del2 = s.find(';', del1 + 1);
				if (del1 != std::string::npos)
				{
					name = s.substr(0, del1);
					path = s.substr(del1 + 1, del2 - (del1 + 1));
					file = s.substr(del2 + 1, s.size() - 1);
				}
			}
		}
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
		bool noZ = false;
	};
	struct BoudingBox2D
	{
		float _minX;
		float _minY;
		float _maxX;
		float _maxY;
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
		
		enum eCullMode
		{
			eCullMode_None = 0,
			eCullMode_ClockwiseFace,
			eCullMode_CounterClockwiseFace,
			__eCullMode_MAX__,
		};

		bool bTransparency = false;
		bool zBuffered = true;
		ZobVector3 color = ZobVector3(1, 1, 1);
		eLightMode lightMode = eLightMode::eLightMode_phong;
		eCullMode cullMode = eCullMode_ClockwiseFace;
	};
	class Ray
	{
	public:
		Ray()
		{
			p = ZobVector3::Vector3Zero;
			n = ZobVector3::Vector3Zero;
		}
		ZobVector3 p;
		ZobVector3 n;
	};
	class Plane
	{
	public:
		float a;
		float b;
		float c;
		float d;
	};
	class Box 
	{
	public:
		ZobVector3 p0;
		ZobVector3 p1;
		ZobVector3 p2;
		ZobVector3 p3;
		ZobVector3 p4;
		ZobVector3 p5;
		ZobVector3 p6;
		ZobVector3 p7;
	};
};

typedef DirectZobType::u8 u8;
typedef DirectZobType::u16 u16;
typedef DirectZobType::uint uint;
typedef DirectZobType::guid u32;
typedef DirectZobType::ulong ulong;
typedef DirectZobType::zobId zobId;
typedef struct DirectZobType::ZobFilePath ZobFilePath;
typedef struct DirectZobType::BufferData BufferData;
typedef struct DirectZobType::Line3D Line3D;
typedef struct DirectZobType::BoudingBox2D BoudingBox2D;
typedef struct DirectZobType::RenderOptions RenderOptions;
typedef class DirectZobType::Ray Ray;
typedef class DirectZobType::Plane Plane;
typedef class DirectZobType::Box Box;
