#ifdef _WINDLL
#include "ZobLightWrapper.h"
#include "Light.h"
#include "Camera.h"
using namespace directZobCLI;

ZobLightWrapper::ZobLightWrapper(directZob::Light* zobObject):ZobObjectWrapper(zobObject)
{
	m_isValid = zobObject != NULL && IsLight();
}

void ZobLightWrapper::SetColor(ManagedVector3^ p)
{		
	directZob::Vector3 v = p->ToVector3();
	directZob::Light* l = (directZob::Light*)m_Instance;
	l->SetColor(&v);
}

ManagedVector3^ ZobLightWrapper::GetColor()
{
	directZob::Vector3 v;
	directZob::Light* l = (directZob::Light*)m_Instance;
	v = l->GetColor();
	return gcnew ManagedVector3(v);
}
float ZobLightWrapper::GetSpotAngle()
{
	directZob::Light* l = (directZob::Light*)m_Instance;
	return l->GetSpotAngle();
}
void ZobLightWrapper::SetSpotAngle(float f)
{
	directZob::Light* l = (directZob::Light*)m_Instance;
	l->SetSpotAngle(f);
}
float ZobLightWrapper::GetIntensity()
{	 
	directZob::Light* l = (directZob::Light*)m_Instance;
	return l->GetIntensity();
}	 
void ZobLightWrapper::SetIntensity(float f)
{	 
	directZob::Light* l = (directZob::Light*)m_Instance;
	l->SetIntensity(f);
}	 
float ZobLightWrapper::GetDistance()
{	 
	directZob::Light* l = (directZob::Light*)m_Instance;
	return l->GetFallOffDistance();
}	 
void ZobLightWrapper::SetDistance(float f)
{	 
	directZob::Light* l = (directZob::Light*)m_Instance;
	l->SetFallOffDistance(f);
}	 
int	 ZobLightWrapper::GetType()
{	 
	directZob::Light* l = (directZob::Light*)m_Instance;
	return (int)l->GetType();
}	 
void ZobLightWrapper::SetType(int t)
{
	directZob::Light* l = (directZob::Light*)m_Instance;
	return l->SetType((directZob::Light::eLightType)t);
}

#endif //_WINDLL