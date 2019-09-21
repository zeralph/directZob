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
		return gcnew ZobObjectWrapper(z);
	}
	ZobObjectWrapper^ ZobObjectManagerWrapper::GetObjectAtCoords(int x, int y)
	{
		uint id = DirectZob::GetInstance()->GetEngine()->GetObjectIdAtCoords(x, y);
		ZobObject* z = m_Instance->GetZobObjectFromPartialId(id);
		if (z)
		{
			return gcnew ZobObjectWrapper(z);
		}
	}

	void ZobObjectManagerWrapper::RemoveZobObject(System::String^ name)
	{
		std::string n;
		MarshalString(name, n);
		ZobObject* z = m_Instance->GetZobObject(n);
		m_Instance->RemoveZobObject(z);
	}

	void ZobObjectManagerWrapper::AddZobObject(System::String^ parent)
	{
		std::string n;
		MarshalString(parent, n);
		ZobObject* z = m_Instance->GetZobObject(n);
		m_Instance->CreateZobObject(z);
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

	bool ZobObjectManagerWrapper::Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent)
	{
		return m_Instance->Reparent((o != nullptr) ? o->GetInstance() : NULL, (parent != nullptr) ? parent->GetInstance() : NULL);
	}
}
#endif //_WINDLL