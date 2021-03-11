#ifdef _WINDLL
#include "ZobObjectWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobLightWrapper.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/Camera.h"
#include "../DirectZob.h"
namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject):ManagedObject(zobObject, false)
	{
		m_id = zobObject->GetId();
	}

	ZobObjectWrapper::~ZobObjectWrapper()
	{
		m_Instance = NULL;
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

	void ZobObjectWrapper::LoadMesh(System::String^ name, System::String^ file, System::String^ path)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			std::string f;
			MarshalString(file, f);
			std::string p;
			MarshalString(path, p);
			z->LoadMesh(n, f, p);
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

	ManagedVector3^ ZobObjectWrapper::GetWorldPosition()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetWorldPosition());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetWorldRotation()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetWorldRotation());
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

	void ZobObjectWrapper::SetWorldTransform(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetWorldPosition(v.x, v.y, v.z);
		}
	}

	void ZobObjectWrapper::LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up, bool addToCurrentRotation)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 l = left->ToVector3();
			ZobVector3 f = forward->ToVector3();
			ZobVector3 u = up->ToVector3();
			z->LookAt(&f, &l, &u, addToCurrentRotation);
		}
	}

	void ZobObjectWrapper::SetWorldRotation(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetWorldRotation(v.x, v.y, v.z);
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
			ZOBGUID::ZobSubType t = z->GetSubType();
			return t == ZOBGUID::ZobSubType::subtype_zobLight;
		}
		return false;
	}

	bool ZobObjectWrapper::IsCamera()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::ZobSubType::subtype_zobCamera;
		}
		return false;
	}

	bool ZobObjectWrapper::IsSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::subtype_sprite;
		}
		return false;
	}

	bool ZobObjectWrapper::HasMesh()
	{
		ZobObject* z = GetInstance();
		if (z->GetMesh())
		{
			return true;
		}
		return false;
	}

	System::Collections::Generic::List<ZobObjectWrapper^>^ ZobObjectWrapper::GetChildren()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			System::Collections::Generic::List<ZobObjectWrapper^>^ list = gcnew System::Collections::Generic::List<ZobObjectWrapper^>();
			const std::vector<ZobObject*>* v = z->GetChildren();
			for (std::vector<ZobObject*>::const_iterator iter = v->begin(); iter != v->end(); iter++)
			{
				ZobObject* z=*iter;
				if (dynamic_cast<Light*>(z))
				{
					list->Add(gcnew ZobLightWrapper(dynamic_cast<Light*>(z)));
				}
				else if (dynamic_cast<Camera*>(z))
				{
					list->Add(gcnew ZobCameraWrapper(dynamic_cast<Camera*>(z)));
				}
				else
				{
					list->Add(gcnew ZobObjectWrapper(z));
				}
			}
			return list;
		}
		return nullptr;
	}

	bool ZobObjectWrapper::IsValid()
	{
		if (m_Instance)
		{		
			if (DirectZob::GetInstance()->GetZobObjectManager()->IsDeleted(m_id))
			{
				m_Instance = NULL;
				return false;
			}
			else
			{
				return true;
			}
		}
		return false;
	}

	void ZobObjectWrapper::GetPhysicComponentInfo(System::String^% type, System::String^% shapeType)
	{
		if (GetInstance())
		{
			std::string cType;
			std::string cShapeType;
			GetInstance()->GetPhysicComponentInfo(cType, cShapeType);
			type = gcnew System::String(cType.c_str());
			shapeType = gcnew System::String(cShapeType.c_str());
		};
	}

	void ZobObjectWrapper::SetPhysicComponentInfo(System::String^ type, System::String^ shapeType)
	{
		std::string cType;
		std::string cShapeType;
		MarshalString(type, cType);
		MarshalString(shapeType, cShapeType);
		if (GetInstance())
		{
			GetInstance()->SetPhysicComponentInfo(cType, cShapeType);
		}
	}


	void ZobObjectWrapper::GetPhysicComponentShapeInfo(float% radius, float% height, float% hx, float% hy, float% hz, System::String^% mesh)
	{
		if (GetInstance())
		{
			float r, h, x, y, z;
			std::string s;
			GetInstance()->GetPhysicComponentShapeInfo(r, h, x, y, z, s);
			radius = r;
			height = h;
			hx = x;
			hy = y;
			hz = z;
			mesh = gcnew System::String(s.c_str());
		}
	}

	void ZobObjectWrapper::SetPhysicComponentShapeInfo(float radius, float height, float hx, float hy, float hz, System::String^ mesh)
	{
		if (GetInstance())
		{
			std::string cMesh;
			MarshalString(mesh, cMesh);
			GetInstance()->SetPhysicComponentShapeInfo(radius, height, hx, hy, hz, cMesh);
		}
	}

	void ZobObjectWrapper::GetPhysicComponentColliderInfo(float% bounciness, float% frictionCoeff, float% massDensity, float% RollingResistance)
	{
		if (GetInstance())
		{
			float b, m, f, r;
			GetInstance()->GetPhysicComponentColliderInfo(b, f, m, r);
			bounciness = b;
			frictionCoeff = f;
			massDensity = m;
			RollingResistance = r;
		}
	}

	void ZobObjectWrapper::SetPhysicComponentColliderInfo(float bounciness, float frictionCoeff, float massDensity, float RollingResistance)
	{
		if (GetInstance())
		{
			GetInstance()->SetPhysicComponentColliderInfo(bounciness, frictionCoeff, massDensity, RollingResistance);
		}
	}

	void ZobObjectWrapper::SetPhysicComponentScaleWithObject(bool b)
	{
		if (GetInstance())
		{
			GetInstance()->SetPhysicComponentScaleWithObject(b);
		}
	}

	void ZobObjectWrapper::GetPhysicComponentScaleWithObject(bool% b)
	{
		if (GetInstance())
		{
			bool cb = false;
			GetInstance()->GetPhysicComponentScaleWithObject(cb);
			b = cb;
		}
	}
}
#endif //_WINDLL