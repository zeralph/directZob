#ifdef _WINDLL
#ifndef DZOB_LIGHT_MANAGER_WRAPPER_H
#define DZOB_LIGHT_MANAGER_WRAPPER_H

#include "ManagedObject.h"
#include "LightManager.h"
#include "DirectZob.h"
#include "ManagedVector3.h"
#include <string>
#include <vector>
namespace directZobCLI
{
	public ref class LightManagerWrapper : public ManagedObject<directZob::LightManager>
	{
	public:

		LightManagerWrapper();
		void			CreateLight(int type);
		void			GetLight(System::String^);

		ManagedVector3^ GetClearColor();
		void			SetClearColor(ManagedVector3^ c);
		ManagedVector3^ GetFogColor();
		void			SetFogColor(ManagedVector3^ c);
		ManagedVector3^ GetAmbientColor();
		void			SetAmbientColor(ManagedVector3^ c);
		int				GetFogType();
		void			SetFogType(int type);
		float			GetFogDistance();
		void			SetFogDistance(float f);
		float			GetFogIntensity();
		void			SetFogIntensity(float d);
		float			GetAmbientIntensity();
		void			SetAmbientIntensity(float d);

	private:

	};
}
#endif //DZOB
#endif //_WINDLL