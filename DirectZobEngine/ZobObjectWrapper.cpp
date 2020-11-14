#ifdef _WINDLL
#include "ZobObjectWrapper.h"
#include "Light.h"
#include "Camera.h"
#include "DirectZob.h"
namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject):ManagedObject(zobObject, false)
	{
		m_isValid = zobObject != NULL;
	}

	ZobObjectWrapper::~ZobObjectWrapper()
	{
		m_Instance = NULL;
		m_isValid = false;
	}

	bool ZobObjectWrapper::IsFromFactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->IsFromFactoryFile();
		}
		return nullptr;
	}
	System::String^ ZobObjectWrapper::FactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return gcnew System::String(z->FactoryFile().c_str());
		}
		return nullptr;
	}

	void ZobObjectWrapper::SaveToFactoryFile(System::String^ file)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string f;
			MarshalString(file, f);
			return z->SaveToFactoryFile(f);
		}
	}

	System::String^ ZobObjectWrapper::GetName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return gcnew System::String(z->GetName().c_str());
		}
		return nullptr;
	}

	System::String^ ZobObjectWrapper::GetFullNodeName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string s;
			z->GetFullNodeName(s);
			return gcnew System::String(s.c_str());
		}
		return nullptr;
	}

	System::String^ ZobObjectWrapper::GetMeshName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return gcnew System::String(z->GetMeshName().c_str());
		}
		return nullptr;
	}

	ManagedRenderOptions^ ZobObjectWrapper::GetRenderOptions()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedRenderOptions^ r = gcnew CLI::ManagedRenderOptions(z->GetRenderOptions());
			return r;
		}
		return nullptr;
	}

	void ZobObjectWrapper::SetMesh(System::String^ name)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			z->SetMesh(n);
		}
	}

	void ZobObjectWrapper::LoadMesh(System::String^ name, System::String^ path)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			std::string p;
			MarshalString(path, p);
			z->LoadMesh(n, p);
		}
	}

	void ZobObjectWrapper::CreateSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			z->CreateSprite();
		}
	}

	void ZobObjectWrapper::SetName(System::String^ name)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			z->SetName(n);
		}
	}

	ManagedVector3^ ZobObjectWrapper::GetTransform()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetPosition());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetRotation()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetRotation());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetLeft()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetLeft());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetUp()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetUp());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetForward()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetForward());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetScale()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetScale());
			return v;
		}
		return nullptr;
	}

	void ZobObjectWrapper::SetTransform(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetPosition(v.x, v.y, v.z);
		}
	}

	void ZobObjectWrapper::LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 l = left->ToVector3();
			ZobVector3 f = forward->ToVector3();
			ZobVector3 u = up->ToVector3();
			z->LookAt(&f, &l, &u);
		}
	}

	void ZobObjectWrapper::SetRotation(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetRotation(v.x, v.y, v.z);
		}
	}

	void ZobObjectWrapper::SetPhysicComponent(int i)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			z->SetPhysicComponent(i);
		}
	}

	void ZobObjectWrapper::SetScale(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetScale(v.x, v.y, v.z);
		}
	}

	int ZobObjectWrapper::GetLightingMode()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetLightingMode();
		}
		return -1;
	}

	void ZobObjectWrapper::SetLightingMode(int lightMode)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			RenderOptions::eLightMode l = (RenderOptions::eLightMode)lightMode;
			z->SetLightingMode(l);
		}
	}

	bool ZobObjectWrapper::IsLight()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZOBGUID::SubType t = z->GetSubType();
			return t == ZOBGUID::SubType::subtype_zobLight;
		}
		return false;
	}

	bool ZobObjectWrapper::IsCamera()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::SubType::subtype_zobCamera;
		}
		return false;
	}

	bool ZobObjectWrapper::IsSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::SubType::subtype_sprite;
		}
		return false;
	}

	bool ZobObjectWrapper::HasMesh()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::SubType::subtype_zobOject;
		}
		return false;
	}

	System::Collections::Generic::List<ZobObjectWrapper^>^ ZobObjectWrapper::GetChildren()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			System::Collections::Generic::List<ZobObjectWrapper^>^ l = gcnew System::Collections::Generic::List<ZobObjectWrapper^>();
			const std::vector<ZobObject*>* v = z->GetChildren();
			for (std::vector<ZobObject*>::const_iterator iter = v->begin(); iter != v->end(); iter++)
			{
				l->Add(gcnew ZobObjectWrapper(*iter));
			}
			return l;
		}
		return nullptr;
	}

	ZobObject* ZobObjectWrapper::GetInstance()
	{
		if (!m_Instance || m_Instance->IsMarkedForDeletion())
		{
			delete(this);
			return nullptr;
		}
		return m_Instance;
	}
}
#endif //_WINDLL