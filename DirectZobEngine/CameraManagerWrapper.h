#ifdef _WINDLL
#ifndef DZOB_CAMERA_MANAGER_WRAPPER_H
#define DZOB_CAMERA_MANAGER_WRAPPER_H


#include "ManagedObject.h"
#include "CameraManager.h"
#include "DirectZob.h"
#include "ManagedVector3.h"
#include <string>
#include <vector>
namespace directZobCLI
{
	public ref class CameraManagerWrapper : public ManagedObject<directZob::CameraManager>
	{
	public:

		CameraManagerWrapper();

		array<System::String^>^ GetCameraList();
		System::String^ 		GetCurrentCamera();
		ManagedVector3^ 		GetCurrentCameraPosition();
		ManagedVector3^ 		GetCurrentCameraTarget();
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
#endif //DZOB
#endif //_WINDLL