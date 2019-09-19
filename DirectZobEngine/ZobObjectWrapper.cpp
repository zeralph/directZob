#include "ZobObjectWrapper.h"

namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject):ManagedObject(zobObject, false)
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

	void ZobObjectWrapper::SetMesh(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		m_Instance->SetMesh(n);
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
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetTransform());
		return v;
	}

	ManagedVector3^ ZobObjectWrapper::GetRotation()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetRotation());
		return v;
	}

	ManagedVector3^ ZobObjectWrapper::GetScale()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetScale());
		return v;
	}

	void ZobObjectWrapper::SetTransform(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetTranslation(v.x, v.y, v.z);
	}

	void ZobObjectWrapper::SetRotation(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetRotation(v.x, v.y, v.z);
	}

	void ZobObjectWrapper::SetScale(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetScale(v.x, v.y, v.z);
	}
}