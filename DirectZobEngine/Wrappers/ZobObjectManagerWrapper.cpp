#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectManagerWrapper.h"
#include "../DirectZob.h"
#include "../ZobObjects/ZobObject.h"
#include "ZobLightWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobSpriteWrapper.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{

	ZobObjectManagerWrapper::ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel):ManagedObject(DirectZob::GetInstance()->GetZobObjectManager(), false)
	{
		m_selectedObject = NULL;
		m_objectTreeviewPanel = objectTreeviewPanel;
		m_objectPropertiesPanel = objectPropertiesPanel;
		m_selectedObjectWrapper = nullptr;
		CreateTreeview();
		CreateObjectview();
		//DirectZobWrapper::OnEditorUpdateEvent += gcnew DirectZobWrapper::OnEditorUpdate(this, &ZobObjectManagerWrapper::OnEditorUpdate);
		DirectZobWrapper::OnEditorUpdateEvent += gcnew DirectZobWrapper::OnEditorUpdate(m_treeView, &CLI::ZobControlTreeview::UpdateZobControl);
	}

	System::String^ ZobObjectManagerWrapper::GetZobObjectList()
	{
		std::string s;
		m_Instance->GetZobObjectList(s);
		return gcnew System::String(s.c_str());
	}

	void ZobObjectManagerWrapper::OnEditorUpdate(Object^ sender, EventArgs^ e)
	{
		m_treeView->Invoke(gcnew Action(m_treeView, &CLI::ZobControlTreeview::UpdateZobControl));
	}

	void ZobObjectManagerWrapper::CreateObjectview()
	{

	}

	void ZobObjectManagerWrapper::CreateTreeview()
	{
		UserControl^ c = gcnew UserControl();
		m_treeView = gcnew CLI::ZobControlTreeview();
		m_treeView->Dock = DockStyle::Fill;
		m_treeView->AfterSelect += gcnew TreeViewEventHandler(this, &ZobObjectManagerWrapper::TreeNodeClick);
		Refresh();
		c->Controls->Add(m_treeView);
		c->Dock = DockStyle::Fill;
		m_objectTreeviewPanel->Controls->Add(c);
	}

	void ZobObjectManagerWrapper::TreeNodeClick(Object^ sender, TreeViewEventArgs^ e)
	{
		//TreeView^ tv = (TreeView^)sender;
		ZobObjectManagerTreeNode^ tn = (ZobObjectManagerTreeNode^)e->Node;// tv->SelectedNode;
		if (tn)
		{
			std::string id;
			MarshalString(tn->m_zobObjectGuid, id);
			m_selectedObject = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id);
			if (m_selectedObject != NULL)
			{
				if (m_selectedObjectWrapper != nullptr)
				{
					delete m_selectedObjectWrapper;
				}
				m_selectedObjectWrapper = gcnew ZobObjectWrapper(m_selectedObject, m_objectPropertiesPanel);
			}
		}
		else
		{
			m_selectedObject = NULL;
		}
	}
	
	void ZobObjectManagerWrapper::Refresh()
	{
		if (GetInstance())
		{
			m_treeView->Nodes->Clear();
			AddZobObjectsRecursive(GetInstance()->GetRootObject(), m_treeView->Nodes);
		}
	}

	void ZobObjectManagerWrapper::AddZobObjectsRecursive(ZobObject* z, TreeNodeCollection^ collection)
	{
		if (z)
		{
			ZobObjectManagerTreeNode^ tn = gcnew ZobObjectManagerTreeNode();
			const char* nameStr = z->GetName().c_str();
			tn->Text = TO_MANAGED_STRING(nameStr);
			std::string idStr = z->ZobGuidToString();
			tn->m_zobObjectGuid = TO_MANAGED_STRING(idStr.c_str());
			collection->Add(tn);
			for (int i = 0; i < z->GetChildren()->size(); i++)
			{
				AddZobObjectsRecursive(z->GetChild(i), tn->Nodes);
			}
		}
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetZobObject(System::String^ name)
	{
		/*
		std::string n;
		MarshalString(name, n);
		ZobObject* z = m_Instance->GetZobObject(n);
		
		if (z->GetSubType() == ZOBGUID::subtype_zobLight)
		{
			return gcnew ZobLightWrapper((Light*)z);
		}
		else if (z->GetSubType() == ZOBGUID::ZobSubType::subtype_zobCamera)
		{
			return gcnew ZobCameraWrapper((Camera*)z);
		}
		else if (z->GetSubType() == ZOBGUID::ZobSubType::subtype_sprite)
		{
			return gcnew ZobSpriteWrapper((ZobSprite*)z);
		}
		return gcnew ZobObjectWrapper(z);
		*/
		return nullptr;
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetObjectAtCoords(int x, int y, eObjectTypes type)
	{
		return nullptr;
		/*
		uint id = DirectZob::GetInstance()->GetEngine()->GetObjectIdAtCoords(x, y);
		ZobObject* z = m_Instance->GetZobObjectFromPartialId(id);
		ZOBGUID::ZobType t;
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
		*/
	}

	void ZobObjectManagerWrapper::RemoveZobObject(ZobObjectWrapper^ z)
	{
		m_Instance->RemoveZobObject(z->GetInstance());
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobObject(ZobObjectWrapper^ parent)
	{
		/*
		ZobObject* p = parent->GetInstance();
		ZobObject* z = m_Instance->CreateZobObject(p);
		return gcnew ZobObjectWrapper(z);
		*/
		return nullptr;
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobSprite(ZobObjectWrapper^ parent)
	{
		/*
		ZobObject* z = m_Instance->CreateZobSprite(parent->GetInstance());
		return gcnew ZobObjectWrapper(z);
		*/
		return nullptr;
	}

	void ZobObjectManagerWrapper::CreateEditorGizmos(System::String^ editorResourcesPath)
	{
		std::string n;
		MarshalString(editorResourcesPath, n);
		m_Instance->CreateEditorGizmos(n);
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::GetRootObject()
	{
		/*
		ZobObject* z = m_Instance->GetRootObject();
		if (z)
		{
			return gcnew ZobObjectWrapper(z);
		}
		*/
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