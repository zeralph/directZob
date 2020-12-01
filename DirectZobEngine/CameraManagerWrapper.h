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

		ref class Ray
		{
		public:
			Ray(ManagedVector3^ p, ManagedVector3^ v)
			{
				_p = p;
				_v = p;
			}
			Ray(ZobVector3* p, ZobVector3* v)
			{
				_p = gcnew CLI::ManagedVector3(p);
				_v = gcnew CLI::ManagedVector3(v);
			}
			ManagedVector3^ p() { return _p; };
			ManagedVector3^ v() { return _v; };
		private:	
			ManagedVector3^ _p;
			ManagedVector3^ _v;
		};

		CameraManagerWrapper();

		cli::array<System::String^>^	GetCameraList();
		System::String^ 				GetCurrentCameraName();
		ZobCameraWrapper^				GetCurrentCamera();
		ManagedVector3^ 				GetCurrentCameraPosition();
		ManagedVector3^ 				GetCurrentCameraTarget();
		Ray^							From2DToWorld(float x, float y);
		bool							From2DToWorldOnPlane(float x, float y, ManagedVector3^ p0, ManagedVector3^ pn, ManagedVector3^ ret);
		void 							SetCurrentCameraPosition(ManagedVector3^ p);
		void 							Rotate(float x, float y, float z);
		void 							Move(float x, float z, float y);
		void 							Zoom(float z);
		void 							SetLookAt(ManagedVector3^ target);
		void 							SetCurrentCamera(System::String^ name);
		void 							CreateCamera();
		void 							CreateEditorCamera();
	private:

	};
}
#endif //_WINDLL