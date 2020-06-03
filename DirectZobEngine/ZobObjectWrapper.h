#ifdef _WINDLL
#ifndef DZOB_ZOB_OBJECT_WRAPPER_H
#define DZOB_ZOB_OBJECT_WRAPPER_H


#include "ManagedObject.h"
#include "ZobObject.h"
#include "ManagedVector3.h"
#include "ManagedRenderOptions.h"
namespace directZobCLI {
	public ref class ZobObjectWrapper : ManagedObject<directZob::ZobObject>
	{
	public:
		ZobObjectWrapper(directZob::ZobObject* zobObject);
		void SetZobObject(directZob::ZobObject* zobObject) { m_Instance = zobObject; }
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
		void CreateSprite();
		bool IsValid() { return m_isValid; };
		void SetLightingMode(int lightMode);
		int GetLightingMode();
		bool IsLight();
		bool IsSprite();
		bool IsCamera();
		bool HasMesh();
	protected:
		bool m_isValid;
	};
}
#endif //DZOB
#endif //_WINDLL 