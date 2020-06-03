#ifdef _WINDLL
#ifndef DZOB_MANAGED_VECTOR3_H
#define DZOB_MANAGED_VECTOR3_H

#include "./Vector3.h"
namespace directZobCLI 
{
	public ref class ManagedVector3
	{
	public:
		ManagedVector3()
		{
			x = 0; y = 0; z = 0;
		}

		ManagedVector3(int x, int y, int z)
		{
			x = (float)x; y = (float)y; z = (float)z;
		}

		ManagedVector3(float x, float y, float z)
		{
			x = x; y = y; z = z;
		}

		ManagedVector3(const directZob::Vector3& v)
		{
			x = v.x / v.w; y = v.y / v.w; z = v.z / v.w;
		}

		ManagedVector3(ManagedVector3^ v)
		{
			x = v->x; 
			y = v->y; 
			z = v->z;
		}

		directZob::Vector3 ToVector3() { return directZob::Vector3(x, y, z); }
		void FromVector3(directZob::Vector3& v) { x = v.x / v.w; y = v.y / v.w; z = v.z / v.w; }
		void Add(ManagedVector3^ v)
		{
			x += v->x;
			y += v->y;
			z += v->z;
		}

		void Mul(float f)
		{
			x *= f;
			y *= f;
			z *= f;
		}
	public:
		float x;
		float y;
		float z;
	};
}
#endif //DZOB
#endif //_WINDLL