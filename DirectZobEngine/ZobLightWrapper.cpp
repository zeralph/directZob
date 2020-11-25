#ifdef _WINDLL
#include "ZobLightWrapper.h"
#include "Light.h"
#include "Camera.h"
namespace CLI
{
	ZobLightWrapper::ZobLightWrapper(Light* zobObject):ZobObjectWrapper(zobObject)
	{
		//m_isValid = zobObject != NULL && IsLight();
	}

	void ZobLightWrapper::SetColor(ManagedVector3^ p)
	{		
		Light* l = (Light*)GetInstance();
		if (l)
		{
			ZobVector3 v = p->ToVector3();
			l->SetColor(&v);
		}
	}

	ManagedVector3^ ZobLightWrapper::GetColor()
	{	
		Light* l = (Light*)GetInstance();
		if (l)
		{
			const ZobVector3* v = l->GetColor();
			return gcnew ManagedVector3(v->x, v->y, v->z);
		}
		return gcnew ManagedVector3(0, 0, 0);
	}
	float ZobLightWrapper::GetSpotAngle()
	{
		Light* l = (Light*)GetInstance();
		if (l)
		{
			return l->GetSpotAngleDeg();
		}
		return -1.0f;
	}
	void ZobLightWrapper::SetSpotAngle(float f)
	{
		Light* l = (Light*)GetInstance();
		if (l)
		{
			l->SetSpotAngle(f);
		}
	}
	float ZobLightWrapper::GetIntensity()
	{	 
		Light* l = (Light*)GetInstance();
		if (l)
		{
			return l->GetIntensity();
		}
		return -1.0f;
	}	 
	void ZobLightWrapper::SetIntensity(float f)
	{	 
		Light* l = (Light*)GetInstance();
		if (l)
		{
			l->SetIntensity(f);
		}
	}	 
	float ZobLightWrapper::GetDistance()
	{	 
		Light* l = (Light*)GetInstance();
		if (l)
		{
			return l->GetFallOffDistance();
		}
		return -1.0f;
	}	 
	void ZobLightWrapper::SetDistance(float f)
	{	 
		Light* l = (Light*)GetInstance();
		if (l)
		{
			l->SetFallOffDistance(f);
		}
	}	 
	int	 ZobLightWrapper::GetType()
	{	 
		Light* l = (Light*)GetInstance();
		if (l)
		{
			return (int)l->GetType();
		}
		return -1;
	}	 
	void ZobLightWrapper::SetType(int t)
	{
		Light* l = (Light*)GetInstance();
		if (l)
		{
			l->SetType((Light::eLightType)t);
		}
	}
}
#endif //_WINDLL