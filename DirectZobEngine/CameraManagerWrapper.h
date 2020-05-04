#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "CameraManager.h"
#include "DirectZob.h"
#include "ManagedVector3.h"
#include <string>
#include <vector>

namespace CLI
{
	public ref class CameraManagerWrapper : public ManagedObject<CameraManager>
	{
	public:

		CameraManagerWrapper();

		array<System::String^>^ GetCameraList();
		System::String^ GetCurrentCamera();
		ManagedVector3^ GetCurrentCameraPosition();
		ManagedVector3^ GetCurrentCameraTarget();
		void SetCurrentCameraPosition(ManagedVector3^ p);
		void RotateAroundAxis(float x, float y);
		void Move(float x, float y);
		void Zoom(float z);
		void SetLookAt(ManagedVector3^ position, ManagedVector3^ target, ManagedVector3^ up);

	private:

	};
}
#endif //_WINDLL