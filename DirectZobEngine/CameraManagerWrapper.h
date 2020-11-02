#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "CameraManager.h"
#include "DirectZob.h"
#include "ManagedVector3.h"
#include "ZobCameraWrapper.h"
#include <string>
#include <vector>

namespace CLI
{
	public ref class CameraManagerWrapper : public ManagedObject<CameraManager>
	{
	public:

		CameraManagerWrapper();

		cli::array<System::String^>^ GetCameraList();
		System::String^ 		GetCurrentCameraName();
		ZobCameraWrapper^		GetCurrentCamera();
		ManagedVector3^ 		GetCurrentCameraPosition();
		ManagedVector3^ 		GetCurrentCameraTarget();
		void					From2DToWorld(ManagedVector3^ p);
		void					From2DToWorldOnPlane(ManagedVector3^ v, ManagedVector3^ p0, ManagedVector3^ pn);
		void 					SetCurrentCameraPosition(ManagedVector3^ p);
		void 					RotateAroundAxis(float x, float y);
		void 					Move(float x, float y);
		void 					Zoom(float z);
		void 					SetLookAt(ManagedVector3^ target);
		void 					SetCurrentCamera(System::String^ name);
		void 					CreateCamera();
		void 					CreateEditorCamera();
	private:

	};
}
#endif //_WINDLL