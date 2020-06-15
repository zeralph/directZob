#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"

namespace CLI {
	public ref class ZobObjectWrapper : ManagedObject<ZobObject>
	{
	public:
		ZobObjectWrapper(ZobObject* zobObject);
		void SetZobObject(ZobObject* zobObject) { m_Instance = zobObject; }
		System::String^ GetName();
		System::String^ GetFullNodeName();
		void SetName(System::String^ name);
		ManagedVector3^ GetTransform();
		ManagedVector3^ GetLeft();
		ManagedVector3^ GetForward();
		ManagedVector3^ GetUp();
		ManagedVector3^ GetRotation();
		ManagedVector3^ GetScale();
		ManagedRenderOptions^ GetRenderOptions();
		void SetTransform(ManagedVector3^ p);
		void SetRotation(ManagedVector3^ p);
		void SetScale(ManagedVector3^ p);
		System::String^ GetMeshName();
		void SetMesh(System::String^ name);
		void LoadMesh(System::String^ name);
		void CreateSprite();
		bool IsValid() { return m_isValid; };
		void SetLightingMode(int lightMode);
		int GetLightingMode();
		bool IsLight();
		bool IsSprite();
		bool IsCamera();
		bool HasMesh();
		void SetPhysicComponent(int i);
	protected:
		bool m_isValid;
	};
}
#endif //_WINDLL 