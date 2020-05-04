#ifdef _WINDLL
#include "LightManagerWrapper.h"
namespace CLI
{
	LightManagerWrapper::LightManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetLightManager(), false)
	{

	}

	void LightManagerWrapper::CreateLight()
	{
		/*return*/ m_Instance->CreateLight();
	}

	void LightManagerWrapper::GetLight(System::String^)
	{
		/*return NULL*/;
	}
}
#endif //_WINDLL