#ifdef _WINDLL
#include "ZobObjectManagerWrapper.h"
#include "DirectZob.h"
#include "ZobObject.h"
#include "ZobLightWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobSpriteWrapper.h"
using namespace directZobCLI;

	ZobObjectManagerWrapper::ZobObjectManagerWrapper():ManagedObject(directZob::DirectZob::GetInstance()->GetZobObjectManager(), false)
	{

	}

	System::String^ ZobObjectManagerWrapper::GetZobObjectList()
	{
		return gcnew System::String(m_Instance->GetZobObjectList().c_str());
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetZobObject(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		directZob::ZobObject* z = m_Instance->GetZobObject(n);
		
		if (z->GetSubType() == directZob::ZOBGUID::SubType::subtype_zobLight)
		{
			return gcnew ZobLightWrapper((directZob::Light*)z);
		}
		else if (z->GetSubType() == directZob::ZOBGUID::SubType::subtype_zobCamera)
		{
			return gcnew ZobCameraWrapper((directZob::Camera*)z);
		}
		else if (z->GetSubType() == directZob::ZOBGUID::SubType::subtype_sprite)
		{
			return gcnew ZobSpriteWrapper((directZob::ZobSprite*)z);
		}
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetObjectAtCoords(int x, int y, eObjectTypes type)
	{
		directZob::uint id = directZob::DirectZob::GetInstance()->GetEngine()->GetObjectIdAtCoords(x, y);
		directZob::ZobObject* z = m_Instance->GetZobObjectFromPartialId(id);
		directZob::ZOBGUID::Type t;
		if (type != eObjectTypes::eObjectTypes_all)
		{
			switch (type)
			{
			default:
			case eObjectTypes::eObjectTypes_scene:
				t = directZob::ZOBGUID::type_scene;
				break;
			case eObjectTypes::eObjectTypes_editor:
				t = directZob::ZOBGUID::type_editor;
				break;
			}
			while (z != NULL && z->GetType() != t)
			{
				z = z->GetParent();
			}
		}
		if (z)
		{
			return gcnew ZobObjectWrapper(z);
		}
		return nullptr;
	}

	void ZobObjectManagerWrapper::RemoveZobObject(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		directZob::ZobObject* z = m_Instance->GetZobObject(n);
		m_Instance->RemoveZobObject(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobObject(System::String^ parent)
	{
		std::string n;
		MarshalString(parent, n);
		directZob::ZobObject* p = m_Instance->GetZobObject(n);
		directZob::ZobObject* z = m_Instance->CreateZobObject(p);
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobSprite(System::String^ parent)
	{
		std::string n;
		MarshalString(parent, n);
		directZob::ZobObject* p = m_Instance->GetZobObject(n);
		directZob::ZobObject* z = m_Instance->CreateZobSprite(p);
		return gcnew ZobObjectWrapper(z);
	}

	void ZobObjectManagerWrapper::CreateEditorGizmos(System::String^ editorResourcesPath)
	{
		std::string n;
		MarshalString(editorResourcesPath, n);
		m_Instance->CreateEditorGizmos(n);
	}
	ZobObjectWrapper^ ZobObjectManagerWrapper::GetEditorGizmos()
	{
		directZob::ZobObject* z = m_Instance->GetEditorGizmos();
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetRootObject()
	{
		directZob::ZobObject* z = m_Instance->GetRootObject();
		return gcnew ZobObjectWrapper(z);
	}

	bool ZobObjectManagerWrapper::Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent)
	{
		return m_Instance->Reparent((o != nullptr) ? o->GetInstance() : NULL, (parent != nullptr) ? parent->GetInstance() : NULL);
	}

#endif //_WINDLL