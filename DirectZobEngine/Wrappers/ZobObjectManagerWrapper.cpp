#ifdef _WINDLL
#include "ZobObjectManagerWrapper.h"
#include "../DirectZob.h"
#include "../ZobObjects/ZobObject.h"
#include "ZobLightWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobSpriteWrapper.h"
namespace CLI
{

	ZobObjectManagerWrapper::ZobObjectManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetZobObjectManager(), false)
	{

	}

	System::String^ ZobObjectManagerWrapper::GetZobObjectList()
	{
		std::string s;
		m_Instance->GetZobObjectList(s);
		return gcnew System::String(s.c_str());
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetZobObject(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		ZobObject* z = m_Instance->GetZobObject(n);
		
		if (z->GetSubType() == ZOBGUID::SubType::subtype_zobLight)
		{
			return gcnew ZobLightWrapper((Light*)z);
		}
		else if (z->GetSubType() == ZOBGUID::SubType::subtype_zobCamera)
		{
			return gcnew ZobCameraWrapper((Camera*)z);
		}
		else if (z->GetSubType() == ZOBGUID::SubType::subtype_sprite)
		{
			return gcnew ZobSpriteWrapper((ZobSprite*)z);
		}
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetObjectAtCoords(int x, int y, eObjectTypes type)
	{
		uint id = DirectZob::GetInstance()->GetEngine()->GetObjectIdAtCoords(x, y);
		ZobObject* z = m_Instance->GetZobObjectFromPartialId(id);
		ZOBGUID::Type t;
		if (type != eObjectTypes::eObjectTypes_all)
		{
			switch (type)
			{
			default:
			case eObjectTypes::eObjectTypes_scene:
				t = ZOBGUID::type_scene;
				break;
			case eObjectTypes::eObjectTypes_editor:
				t = ZOBGUID::type_editor;
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

	void ZobObjectManagerWrapper::RemoveZobObject(ZobObjectWrapper^ z)
	{
		m_Instance->RemoveZobObject(z->GetInstance());
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobObject(ZobObjectWrapper^ parent)
	{
		ZobObject* p = parent->GetInstance();
		ZobObject* z = m_Instance->CreateZobObject(p);
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobSprite(ZobObjectWrapper^ parent)
	{
		ZobObject* z = m_Instance->CreateZobSprite(parent->GetInstance());
		return gcnew ZobObjectWrapper(z);
	}

	void ZobObjectManagerWrapper::CreateEditorGizmos(System::String^ editorResourcesPath)
	{
		std::string n;
		MarshalString(editorResourcesPath, n);
		m_Instance->CreateEditorGizmos(n);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetRootObject()
	{
		ZobObject* z = m_Instance->GetRootObject();
		if (z)
		{
			return gcnew ZobObjectWrapper(z);
		}
		return nullptr;
	}

	bool ZobObjectManagerWrapper::Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent)
	{
		return m_Instance->Reparent((o != nullptr) ? o->GetInstance() : NULL, (parent != nullptr) ? parent->GetInstance() : NULL);
	}

	void ZobObjectManagerWrapper::SaveTransforms()
	{
		m_Instance->SaveTransforms();
	}

	void ZobObjectManagerWrapper::RestoreTransforms()
	{
		m_Instance->RestoreTransforms();
	}
}
#endif //_WINDLL