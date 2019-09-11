#include "ZobObjectInterface.h"

namespace CLI
{
	ZobObjectInterface::ZobObjectInterface(ZobObject* zobObject):ManagedObject(zobObject, false)
	{
	}

	System::String^ ZobObjectInterface::GetName()
	{
		if (m_Instance != NULL)
		{
			return gcnew System::String(m_Instance->GetName().c_str());
		}
		return gcnew System::String("");
	}

	System::String^ ZobObjectInterface::GetMeshName()
	{
		return gcnew System::String(m_Instance->GetMeshName().c_str());
	}

	void ZobObjectInterface::SetMesh(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		m_Instance->SetMesh(n);
	}

	void ZobObjectInterface::SetName(System::String^ name)
	{
		if (m_Instance != NULL)
		{
			std::string n;
			MarshalString(name, n);
			m_Instance->SetName(n);
		}
	}

	ManagedVector3^ ZobObjectInterface::GetTransform()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetTransform());
		return v;
	}

	ManagedVector3^ ZobObjectInterface::GetRotation()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetRotation());
		return v;
	}

	ManagedVector3^ ZobObjectInterface::GetScale()
	{
		ManagedVector3^ v = gcnew CLI::ManagedVector3(m_Instance->GetScale());
		return v;
	}

	void ZobObjectInterface::SetTransform(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetTranslation(v.x, v.y, v.z);
	}

	void ZobObjectInterface::SetRotation(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetRotation(v.x, v.y, v.z);
	}

	void ZobObjectInterface::SetScale(ManagedVector3^ p)
	{
		Vector3 v = p->ToVector3();
		m_Instance->SetScale(v.x, v.y, v.z);
	}
}