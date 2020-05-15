#ifdef _WINDLL
#include "ZobLightWrapper.h"
#include "Light.h"
#include "Camera.h"
namespace CLI
{
	ZobLightWrapper::ZobLightWrapper(Light* zobObject):ZobObjectWrapper(zobObject)
	{
		m_isValid = zobObject != NULL && IsLight();
	}

	void ZobLightWrapper::SetColor(ManagedVector3^ p)
	{		
		Vector3 v = p->ToVector3();
		Light* l = (Light*)m_Instance;
		l->SetColor(&v);
	}

	ManagedVector3^ ZobLightWrapper::GetColor()
	{
		Vector3 v;
		Light* l = (Light*)m_Instance;
		v = l->GetColor();
		return gcnew ManagedVector3(v);
	}
	float ZobLightWrapper::GetSpotAngle()
	{
		Light* l = (Light*)m_Instance;
		return l->GetSpotAngle();
	}
	void ZobLightWrapper::SetSpotAngle(float f)
	{
		Light* l = (Light*)m_Instance;
		l->SetSpotAngle(f);
	}
	float ZobLightWrapper::GetIntensity()
	{	 
		Light* l = (Light*)m_Instance;
		return l->GetIntensity();
	}	 
	void ZobLightWrapper::SetIntensity(float f)
	{	 
		Light* l = (Light*)m_Instance;
		l->SetIntensity(f);
	}	 
	float ZobLightWrapper::GetDistance()
	{	 
		Light* l = (Light*)m_Instance;
		return l->GetFallOffDistance();
	}	 
	void ZobLightWrapper::SetDistance(float f)
	{	 
		Light* l = (Light*)m_Instance;
		l->SetFallOffDistance(f);
	}	 
	int	 ZobLightWrapper::GetType()
	{	 
		Light* l = (Light*)m_Instance;
		return (int)l->GetType();
	}	 
	void ZobLightWrapper::SetType(int t)
	{
		Light* l = (Light*)m_Instance;
		return l->SetType((Light::eLightType)t);
	}
}
#endif //_WINDLL