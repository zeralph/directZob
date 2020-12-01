#ifdef _WINDLL
#pragma once
#include "../Rendering/ZobVector3.h"

namespace CLI {
	public ref class ManagedVector3
	{
	public:
		ManagedVector3()
		{
			x = 0; y = 0; z = 0;
		}

		//ManagedVector3(int x, int y, int z)
		//{
		//	x = (float)x; y = (float)y; z = (float)z;
		//}

		ManagedVector3(float x, float y, float z)
		{
			this->x = x; this->y = y; this->z = z;
		}

		ManagedVector3(const ZobVector3& v)
		{
			x = v.x / v.w; y = v.y / v.w; z = v.z / v.w;
		}

		ManagedVector3(const ManagedVector3^ v)
		{
			this->x = v->x; 
			this->y = v->y;
			this->z = v->z;
		}

		ZobVector3 ToVector3() { return ZobVector3(x, y, z); }
		void FromVector3(ZobVector3& v) { x = v.x / v.w; y = v.y / v.w; z = v.z / v.w; }
		inline void Add(const ManagedVector3^ v)
		{
			x += v->x;
			y += v->y;
			z += v->z;
		}

		inline void Mul(float f)
		{
			x *= f;
			y *= f;
			z *= f;
		}

		inline float sqrtLength()
		{
			return (float)sqrt((double)x * (double)x + (double)y * (double)y + (double)z * (double)z);
		}

		inline void Normalize()
		{
			float f = sqrtLength();
			if (f != 0.0f)
			{
				x /= f;
				y /= f;
				z /= f;
			}
		}

		static bool operator==(const ManagedVector3^ v1, const ManagedVector3^ v2) { return (v1->x == v2->x && v1->y == v2->y && v1->z == v2->z); }
		static bool operator!=(const ManagedVector3^ v1, const ManagedVector3^ v2) { return (v1->x != v2->x || v1->y != v2->y || v1->z != v2->z); }

		float Dot(const ManagedVector3^ v)
		{
			return x * v->x + y * v->y + z * v->z;
		}


		ManagedVector3^ Copy()
		{ 
			return gcnew ManagedVector3(this->x, this->y, this->z); 
		}

		ManagedVector3^ Cross(const ManagedVector3^ v)
		{
			ManagedVector3^ r = gcnew ManagedVector3(
				this->y * v->z - this->z * v->y,
				this->z * v->x - this->x * v->z,
				this->x * v->y - this->y * v->x);
			return r;
		};
	public:
		float x;
		float y;
		float z;
	};
}
#endif //_WINDLL