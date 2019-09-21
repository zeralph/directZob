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
		//			7			6			5			4			3			2			1			0
		//	 ZBuffer|    Lighting mode      |

		enum eLightMode
		{
			eLightMode_none = 0,
			eLightMode_flat,
			eLightMode_gouraud,
		};
		char flag = 0xFF;
		inline const bool ZBuffered() const { return flag & 0x80; }
		void ZBuffered(const bool b)
		{
			if (b)
			{
				flag |= 0x80;
			}
			else
			{
				flag &= 0x7F;
			}
		}
		inline const eLightMode LightMode() const 
		{ 
			int c = (flag & 0x60) >> 5;
			return (eLightMode)c;
		}
		void LightMode(eLightMode l)
		{
			flag &= 0x9F;
			int c = (int)l;
			c = c << 5;
			flag |= c;
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