#ifdef _WINDLL
#include "ZobObjectWrapper.h"
#include "Light.h"
#include "Camera.h"
using namespace directZobCLI;

ZobObjectWrapper::ZobObjectWrapper(directZob::ZobObject* zobObject):ManagedObject(zobObject, false)
{
	m_isValid = zobObject != NULL;
}

System::String^ ZobObjectWrapper::GetName()
{
	if (m_Instance != NULL)
	{
		return gcnew System::String(m_Instance->GetName().c_str());
	}
	return gcnew System::String("");
}

System::String^ ZobObjectWrapper::GetFullNodeName()
{
	std::string s;
	m_Instance->GetFullNodeName(s);
	return gcnew System::String(s.c_str());
}

System::String^ ZobObjectWrapper::GetMeshName()
{
	return gcnew System::String(m_Instance->GetMeshName().c_str());
}

ManagedRenderOptions^ ZobObjectWrapper::GetRenderOptions()
{
	ManagedRenderOptions^ r = gcnew directZobCLI::ManagedRenderOptions(m_Instance->GetRenderOptions());
	return r;
}

void ZobObjectWrapper::SetMesh(System::String^ name)
{
	std::string n;
	MarshalString(name, n);
	m_Instance->SetMesh(n);
}

void ZobObjectWrapper::CreateSprite()
{
	m_Instance->CreateSprite();
}

void ZobObjectWrapper::SetName(System::String^ name)
{
	if (m_Instance != NULL)
	{
		std::string n;
		MarshalString(name, n);
		m_Instance->SetName(n);
	}
}

ManagedVector3^ ZobObjectWrapper::GetTransform()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetTransform());
	return v;
}

ManagedVector3^ ZobObjectWrapper::GetRotation()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetRotation());
	return v;
}

ManagedVector3^ ZobObjectWrapper::GetLeft()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetLeft());
	return v;
}

ManagedVector3^ ZobObjectWrapper::GetUp()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetUp());
	return v;
}

ManagedVector3^ ZobObjectWrapper::GetForward()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetForward());
	return v;
}

ManagedVector3^ ZobObjectWrapper::GetScale()
{
	ManagedVector3^ v = gcnew directZobCLI::ManagedVector3(m_Instance->GetScale());
	return v;
}

void ZobObjectWrapper::SetTransform(ManagedVector3^ p)
{
	directZob::Vector3 v = p->ToVector3();
	m_Instance->SetTranslation(v.x, v.y, v.z);
}

void ZobObjectWrapper::SetRotation(ManagedVector3^ p)
{
	directZob::Vector3 v = p->ToVector3();
	m_Instance->SetRotation(v.x, v.y, v.z);
}

void ZobObjectWrapper::SetScale(ManagedVector3^ p)
{
	directZob::Vector3 v = p->ToVector3();
	m_Instance->SetScale(v.x, v.y, v.z);
}

int ZobObjectWrapper::GetLightingMode()
{
	return m_Instance->GetLightingMode();
}

void ZobObjectWrapper::SetLightingMode(int lightMode)
{
	directZob::RenderOptions::eLightMode l = (directZob::RenderOptions::eLightMode)lightMode;
	m_Instance->SetLightingMode(l);
}

bool ZobObjectWrapper::IsLight()
{
	directZob::ZOBGUID::SubType t = m_Instance->GetSubType();
	return t == directZob::ZOBGUID::SubType::subtype_zobLight;
}

bool ZobObjectWrapper::IsCamera()
{
	return m_Instance->GetSubType() == directZob::ZOBGUID::SubType::subtype_zobCamera;
}

bool ZobObjectWrapper::IsSprite()
{
	return m_Instance->GetSubType() == directZob::ZOBGUID::SubType::subtype_sprite;
}

bool ZobObjectWrapper::HasMesh()
{
	return m_Instance->GetSubType() == directZob::ZOBGUID::SubType::subtype_zobOject;
}

#endif //_WINDLL