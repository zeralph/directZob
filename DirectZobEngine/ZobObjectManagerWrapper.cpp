#ifdef _WINDLL
#include "ZobObjectManagerWrapper.h"
#include "DirectZob.h"
#include "ZobObject.h"
namespace CLI
{

	ZobObjectManagerWrapper::ZobObjectManagerWrapper():ManagedObject(DirectZob::GetInstance()->GetZobObjectManager(), false)
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
		ZobObject* z = m_Instance->GetZobObject(n);
		
		if (z->GetSubType() == ZOBGUID::SubType::subtype_zobLight)
		{
			return gcnew ZobLightWrapper((Light*)z);
		}
		else if (z->GetSubType() == ZOBGUID::SubType::subtype_zobCamera)
		{
			return gcnew ZobCameraWrapper((Camera*)z);
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

	void ZobObjectManagerWrapper::RemoveZobObject(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		ZobObject* z = m_Instance->GetZobObject(n);
		m_Instance->RemoveZobObject(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobObject(System::String^ parent)
	{
		std::string n;
		MarshalString(parent, n);
		ZobObject* p = m_Instance->GetZobObject(n);
		ZobObject* z = m_Instance->CreateZobObject(p);
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
		ZobObject* z = m_Instance->GetEditorGizmos();
		return gcnew ZobObjectWrapper(z);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetRootObject()
	{
		ZobObject* z = m_Instance->GetRootObject();
		return gcnew ZobObjectWrapper(z);
	}

	bool ZobObjectManagerWrapper::Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent)
	{
		return m_Instance->Reparent((o != nullptr) ? o->GetInstance() : NULL, (parent != nullptr) ? parent->GetInstance() : NULL);
	}
}
#endif //_WINDLL