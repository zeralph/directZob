#ifdef _WINDLL
#include "LightManagerWrapper.h"
namespace CLI
{
	LightManagerWrapper::LightManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetLightManager(), false)
	{

	}

	void LightManagerWrapper::CreateLight(int type)
	{
		/*return*/ m_Instance->CreateLight((Light::eLightType)type);
	}

	void LightManagerWrapper::GetLight(System::String^)
	{
		/*return NULL*/;
	}

	ManagedVector3^ LightManagerWrapper::GetClearColor()
	{
		const ZobVector3* c = m_Instance->GetClearColor();
		ManagedVector3^ v = gcnew CLI::ManagedVector3(c);
		return v;
	}
	void LightManagerWrapper::SetClearColor(ManagedVector3^ c)
	{
		ZobVector3 v = c->ToVector3();
		m_Instance->SetClearColor(&v);
	}
	ManagedVector3^ LightManagerWrapper::GetFogColor()
	{
		const ZobVector3* c = m_Instance->GetFogColor();
		ManagedVector3^ v = gcnew CLI::ManagedVector3(c);
		return v;
	}
	void LightManagerWrapper::SetFogColor(ManagedVector3^ c)
	{
		ZobVector3 v = c->ToVector3();
		m_Instance->SetFogColor(&v);
	}
	ManagedVector3^ LightManagerWrapper::GetAmbientColor()
	{
		const ZobVector3* c = m_Instance->GetAmbientColor();
		ManagedVector3^ v = gcnew CLI::ManagedVector3(c);
		return v;
	}
	void LightManagerWrapper::SetAmbientColor(ManagedVector3^ c)
	{
		ZobVector3 v = c->ToVector3();
		m_Instance->SetAmbientColor(&v);
	}
	int	LightManagerWrapper::GetFogType()
	{
		return (int)m_Instance->GetFogType();
	}
	void LightManagerWrapper::SetFogType(int type)
	{
		m_Instance->SetFogType((eFogType)type);
	}
	float LightManagerWrapper::GetFogDistance()
	{
		return m_Instance->GetFogDistance();
	}
	void LightManagerWrapper::SetFogDistance(float f)
	{
		return m_Instance->SetFogDistance(f);
	}
	float LightManagerWrapper::GetFogIntensity()
	{
		return m_Instance->GetFogDensity();
	}
	void LightManagerWrapper::SetFogIntensity(float d)
	{
		m_Instance->SetFogDensity(d);
	}
	float LightManagerWrapper::GetAmbientIntensity()
	{
		return m_Instance->GetAmbientColorIntensity();
	}
	void LightManagerWrapper::SetAmbientIntensity(float d)
	{
		m_Instance->SetAmbientColorIntensity(d);
	}
}
#endif //_WINDLL