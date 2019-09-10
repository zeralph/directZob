#pragma once
#include "ManagedObject.h"
#include "ZobObject.h"
#include "ManagedVector3.h"

namespace CLI {
	public ref class ZobObjectInterface : ManagedObject<ZobObject>
	{
	public:
		ZobObjectInterface(ZobObject* zobObject);
		void SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		System::String^ GetName();
		void SetName(System::String^ name);
		ManagedVector3^ GetTransform();
		ManagedVector3^ GetRotation();
		ManagedVector3^ GetScale();
		void SetTransform(ManagedVector3^ p);
		void SetRotation(ManagedVector3^ p);
		void SetScale(ManagedVector3^ p);
	};
}