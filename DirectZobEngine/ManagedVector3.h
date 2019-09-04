#pragma once
#include "Vector3.h"

namespace CLI {
	public ref class ManagedVector3
	{
	public:
		ManagedVector3()
		{
			x = 0; y = 0; z = 0;
		}

		ManagedVector3(float x, float y, float z)
		{
			x = x; y = y; z = z;
		}

		ManagedVector3(const Vector3& v)
		{
			x = v.x; y = v.y; z = v.z;
		}

		Vector3 ToVector3() { return Vector3(x, y, z); }
		void FromVector3(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	public:
		float x;
		float y;
		float z;
	};
}