#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "LightManager.h"
#include "DirectZob.h"
#include "ManagedVector3.h"
#include <string>
#include <vector>

namespace CLI
{
	public ref class LightManagerWrapper : public ManagedObject<LightManager>
	{
	public:

		LightManagerWrapper();
		void CreateLight();
		void GetLight(System::String^);
	private:

	};
}
#endif //_WINDLL