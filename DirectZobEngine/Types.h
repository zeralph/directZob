#pragma once
#include <stddef.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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
		uint* oBuffer;
	};

	struct Line2D
	{
		float xa;
		float xb;
		float ya;
		float yb;
		float c;
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
		bool zBuffered = true;
		eLightMode lightMode = eLightMode::eLightMode_phong;
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
	};
};

typedef DirectZobType::u8 u8;
typedef DirectZobType::uint uint;
typedef DirectZobType::ulong ulong;
typedef unsigned long long zobId;
typedef struct DirectZobType::BufferData BufferData;
typedef struct DirectZobType::Line2D Line2D;
typedef struct DirectZobType::RenderOptions RenderOptions;