#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectManagerWrapper.h"
#include "../DirectZob.h"
#include "../ZobObjects/ZobObject.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{

	ZobObjectManagerWrapper::ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel):ManagedObject(DirectZob::GetInstance()->GetZobObjectManager(), false)
	{
		m_selectedObject = NULL;
		m_objectTreeviewPanel = objectTreeviewPanel;
		m_objectPropertiesPanel = objectPropertiesPanel;
		m_selectedObjectWrapper = nullptr;
		m_draggedNode = nullptr;
		CreateTreeview();
		CreateObjectview();
		CreateNodeMenu();
		m_editorGizmos = new ZobObjectsEditor();
		DirectZobWrapperEvents::OnNewSceneEvent += gcnew DirectZobWrapperEvents::OnNewScene(this, &ZobObjectManagerWrapper::OnNewScene);
		DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(m_treeView, &CLI::ZobControlTreeview::UpdateZobControl);
		m_bShowAllNodes = false;
	}

	System::String^ ZobObjectManagerWrapper::GetZobObjectList()
	{
		std::string s;
		m_Instance->GetZobObjectList(s);
		return gcnew System::String(s.c_str());
	}

	void ZobObjectManagerWrapper::EditorUpdate()
	{
		m_treeView->Invoke(gcnew Action(m_treeView, &CLI::ZobControlTreeview::UpdateZobControl));
		if (m_selectedObjectWrapper)
		{
			m_selectedObjectWrapper->EditorUpdate();
		}
	}

	void ZobObjectManagerWrapper::CreateNodeMenu()
	{
		m_nodeMenu = gcnew ContextMenuStrip();
		ToolStripMenuItem^ itAdd = gcnew ToolStripMenuItem("Add object");
		itAdd->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::AddZobObject);
		m_nodeMenu->Items->Add(itAdd);
		ToolStripMenuItem^ itRemove = gcnew ToolStripMenuItem("Remove object");
		itRemove->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::RemoveZobObject);
		m_nodeMenu->Items->Add(itRemove);
		ToolStripMenuItem^ itBehavior = gcnew ToolStripMenuItem("Add behavior");
		CreateBehaviorNodeMenu(itBehavior);
		m_nodeMenu->Items->Add(itBehavior);
		ToolStripMenuItem^ itZoom = gcnew ToolStripMenuItem("Zoom");
		itZoom->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::ZoomToZobObject);
		m_nodeMenu->Items->Add(itZoom);
		//m_treeView->ContextMenuStrip = m_nodeMenu;
	}

	void ZobObjectManagerWrapper::CreateBehaviorNodeMenu(ToolStripMenuItem^ parent)
	{
		int l = ZobBehavior::eBehaviorType::__eBehavior_MAX__;
		for (int i = 0; i < l; i++)
		{
			String^ s = gcnew String(ZobBehaviorFactory::eBehaviorTypeStr[i]);
			ToolStripItem^ it = gcnew ToolStripMenuItem(s);
			it->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::AddZobBehavior);
			parent->DropDownItems->Add(it);
		}
	}

	void ZobObjectManagerWrapper::AddZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		ZobObject* p = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id);
		DirectZob::GetInstance()->GetZobObjectManager()->CreateZobObject(p);
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
	}

	void ZobObjectManagerWrapper::RemoveZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		ZobObject* p = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id);
		if (m_selectedObject == p)
		{
			SelectObject(p->GetParent());
		}
		DirectZob::GetInstance()->GetZobObjectManager()->RemoveZobObject(p);
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
	}
	void ZobObjectManagerWrapper::ZoomToZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		ZobObject* p = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id);
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCamera(std::string("EditorCamera"));
		if (c)
		{
			ZobVector3 pos = p->GetWorldPosition();
			c->SetTarget(&pos);
		}
	}
	void ZobObjectManagerWrapper::AddZobBehavior(Object^ sender, EventArgs^ e)
	{
		if (m_selectedObject)
		{
			ToolStripMenuItem^ it = (ToolStripMenuItem^)sender;
			int l = ZobBehavior::eBehaviorType::__eBehavior_MAX__;
			for (int i = 0; i < l; i++)
			{
				String^ s = gcnew String(ZobBehaviorFactory::eBehaviorTypeStr[i]);
				if (s == it->Text)
				{
					ZobBehaviorFactory::CreateBehavior(m_selectedObject, ZobBehaviorFactory::eBehaviorTypeStr[i]);
					m_selectedObjectWrapper->Refresh();
					return;
				}
			}
		}
	}

	void ZobObjectManagerWrapper::CreateObjectview()
	{

	}

	void ZobObjectManagerWrapper::CreateTreeview()
	{
		UserControl^ c = gcnew UserControl();
		m_treeView = gcnew CLI::ZobControlTreeview();
		m_treeView->Dock = DockStyle::Fill;
		m_treeView->NodeMouseClick += gcnew TreeNodeMouseClickEventHandler(this, &ZobObjectManagerWrapper::TreeNodeClick);
		m_treeView->ItemDrag += gcnew ItemDragEventHandler(this, &ZobObjectManagerWrapper::ItemDrag);
		m_treeView->DragDrop += gcnew DragEventHandler(this, &ZobObjectManagerWrapper::DragDrop);
		m_treeView->DragEnter += gcnew DragEventHandler(this, &ZobObjectManagerWrapper::DragEnter);
		m_treeView->DragOver += gcnew DragEventHandler(this, &ZobObjectManagerWrapper::DragOver);
		m_treeView->DragLeave += gcnew EventHandler(this, &ZobObjectManagerWrapper::DragLeave);
		m_treeView->AllowDrop = true;
		m_treeView->MouseHover += gcnew EventHandler(this, &ZobObjectManagerWrapper::TreeNodeMouseHover);
		m_treeView->BeforeSelect += gcnew TreeViewCancelEventHandler(this, &ZobObjectManagerWrapper::TreeNodeBeforeSelect);
		m_treeView->ShowNodeToolTips = true;
		Refresh();
		c->Controls->Add(m_treeView);
		c->Dock = DockStyle::Fill;
		m_objectTreeviewPanel->Controls->Add(c);
	}

	void ZobObjectManagerWrapper::ItemDrag(Object^ sender, ItemDragEventArgs^ e)
	{
		m_draggedNode = (ZobControlTreeNode^)e->Item;
		m_treeView->DoDragDrop(m_draggedNode, DragDropEffects::Move);
	}
	void ZobObjectManagerWrapper::DragDrop(Object^ sender, DragEventArgs^ e)
	{
		Point p = Point(e->X, e->Y);
		Point targetPoint = m_treeView->PointToClient(p);
		ZobControlTreeNode^ targetNode = (ZobControlTreeNode^)m_treeView->GetNodeAt(targetPoint);
		ZobControlTreeNode^ draggedNode = m_draggedNode;
		if (draggedNode && targetNode)
		{
			Reparent(draggedNode->ToolTipText, targetNode->ToolTipText);
			ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
			targetNode->Expand();
		}
	}
	void ZobObjectManagerWrapper::DragEnter(Object^ sender, DragEventArgs^ e)
	{
		e->Effect = e->AllowedEffect;
	}
	void ZobObjectManagerWrapper::DragOver(Object^ sender, DragEventArgs^ e)
	{
		Point p = Point(e->X, e->Y);
		Point targetPoint = m_treeView->PointToClient(p);
		m_treeView->SelectedNode = m_treeView->GetNodeAt(targetPoint);
	}

	void ZobObjectManagerWrapper::DragLeave(Object^ sender, EventArgs^ e)
	{
		m_draggedNode = nullptr;
	}

	void ZobObjectManagerWrapper::TreeNodeBeforeSelect(Object^ sender, TreeViewCancelEventArgs^ e)
	{
		if (e->Node)
		{
			ZobControlTreeNode^ z = (ZobControlTreeNode^)e->Node;
			if (z && !z->IsEditable())
			{
				e->Cancel = true;
			}
		}
	}

	void ZobObjectManagerWrapper::TreeNodeMouseHover(Object^ sender, EventArgs^ e)
	{
	}

	void ZobObjectManagerWrapper::OnNewScene()
	{
		SelectObject(NULL);
		m_treeView->Nodes->Clear();
		//m_editorGizmos->AddEditorGizmos();
	}

	void ZobObjectManagerWrapper::TreeNodeClick(Object^ sender, TreeNodeMouseClickEventArgs^ e)
	{
		ZobControlTreeNode^ tn = (ZobControlTreeNode^)e->Node;
		if (tn)
		{
			if (tn->IsEditable())
			{
				m_treeView->SelectedNode = e->Node;
				if (e->Button == MouseButtons::Left)
				{
					SelectObject(GetZobObject(tn->m_zobObjectGuid));
				}
				else if (e->Button == MouseButtons::Right)
				{
					int x = tn->Bounds.X + tn->Bounds.Width;
					int y = tn->Bounds.Y + tn->Bounds.Height;
					Point point = Point(x, y);
					Point absPoint = m_treeView->PointToScreen(point);
					m_nodeMenu->Top = absPoint.Y;
					//m_nodeMenu->Location = absPoint;
					m_nodeMenu->Show();
					m_nodeMenu->Location = absPoint;
				}
			}
		}
		else
		{
			SelectObject(NULL);
		}
	}
	
	void ZobObjectManagerWrapper::Refresh()
	{
		if (GetInstance())
		{
			m_treeView->Nodes->Clear();
			ZobObject* root = GetInstance()->GetRootObject();
			AddZobObjectsRecursive(root, m_treeView->Nodes);
		}
	}

	void ZobObjectManagerWrapper::AddEditorGizmos()
	{
			m_editorGizmos->AddEditorGizmos();
	}

	void ZobObjectManagerWrapper::ReScan()
	{
		bool showAllNodes = false;
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, showAllNodes);
	}

	void ZobObjectManagerWrapper::ReScan(ZobControlTreeNode^ n, bool showAllNodes)
	{

		ZobObject* z = GetZobObject(n->m_zobObjectGuid);
		if (z && !z->IsMarkedForDeletion())
		{
			if (m_selectedObject == z)
			{
				m_treeView->SelectedNode = n;
				//n->SelectedImageIndex()
			}
			const std::vector<ZobObject*>* v = z->GetChildren();
			List<String^>^ l = gcnew List<String^>();
			for (int i = 0; i < v->size(); i++)
			{
				ZobObject* child = v->at(i);
				if (!child->IsMarkedForDeletion())
				{
					String^ guid = TO_MANAGED_STRING(child->ZobGuidToString().c_str());
					bool isEditable = child->GetType() == ZOBGUID::type_scene;
					//if (isEditable || showAllNodes)
					{
						l->Add(guid);
						ZobControlTreeNode^ childNode = n->GetChildNode(guid);
						if (!childNode)
						{
							ZobControlTreeNode^ newNode = gcnew ZobControlTreeNode(guid, isEditable);
							newNode->Text = TO_MANAGED_STRING(child->GetName().c_str());
							n->Nodes->Add(newNode);
							n->Expand();
							ReScan(newNode, m_bShowAllNodes);
						}
						else
						{
							childNode->Text = TO_MANAGED_STRING(child->GetName().c_str());
							ReScan(childNode, m_bShowAllNodes);
						}
					}
				}
			}
			for (int i = n->Nodes->Count-1; i >= 0 ; i--)
			{
				ZobControlTreeNode^ oldNode = (ZobControlTreeNode ^)n->Nodes[i];
				if (!l->Contains(oldNode->m_zobObjectGuid))
				{
					n->Nodes[i]->Remove();
				}
			}
		}
	}

	void ZobObjectManagerWrapper::AddZobObjectsRecursive(ZobObject* z, TreeNodeCollection^ collection)
	{
		if (z)
		{
			String^ guidStr = TO_MANAGED_STRING(z->ZobGuidToString().c_str());
			bool isEditable = z->GetType() == ZOBGUID::type_scene;
			ZobControlTreeNode^ tn = gcnew ZobControlTreeNode(guidStr, isEditable);
			const char* nameStr = z->GetName().c_str();
			tn->Text = TO_MANAGED_STRING(nameStr);
			collection->Add(tn);
			for (int i = 0; i < z->GetChildren()->size(); i++)
			{
				AddZobObjectsRecursive(z->GetChild(i), tn->Nodes);
			}
		}
	}

	ZobObject* ZobObjectManagerWrapper::GetZobObject(System::String^ guid)
	{
		std::string id;
		MarshalString(guid, id);
		return DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id);
	}

	void ZobObjectManagerWrapper::RemoveZobObject(ZobObjectWrapper^ z)
	{
		m_Instance->RemoveZobObject(z->GetInstance());
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::CreateZobObject()
	{
		if (GetInstance())
		{
			ZobObject* z = NULL;
			if (m_selectedObject)
			{
				z = GetInstance()->CreateZobObject(m_selectedObject);
			}
			else
			{
				z = GetInstance()->CreateZobObject(GetInstance()->GetRootObject());
			}
			if(z)
			{
				SelectObject(z);
				ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
				return m_selectedObjectWrapper;
			}
		}
		return nullptr;
	}

	void ZobObjectManagerWrapper::SelectObject(ZobObject* z)
	{
		if (m_selectedObjectWrapper)
		{
			delete m_selectedObjectWrapper; 
			m_selectedObjectWrapper = nullptr;
		}
		if (z)
		{
			m_selectedObject = z;
			m_selectedObjectWrapper = gcnew ZobObjectWrapper(z, m_objectPropertiesPanel);
			m_editorGizmos->SetParent(z);
			OnObjectSelectedEvent(m_selectedObjectWrapper);
			ReScan();
			String^ t = gcnew String(z->GetName().c_str());
			string outStr;
			ManagedObject::MarshalString(t, outStr);
			DirectZob::LogInfo("Selected %s", outStr.c_str());
		}
		else
		{
			if (m_selectedObjectWrapper)
			{
				delete m_selectedObjectWrapper;
			}
			m_selectedObject = NULL;			
			OnObjectSelectedEvent(nullptr);
		}
	}

	ZobObjectWrapper^ ZobObjectManagerWrapper::AddZobSprite(ZobObjectWrapper^ parent)
	{
		/*
		ZobObject* z = m_Instance->CreateZobSprite(parent->GetInstance());
		return gcnew ZobObjectWrapper(z);
		*/
		return nullptr;
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

	bool ZobObjectManagerWrapper::Reparent(String^ object, String^ parent)
	{
		if (GetInstance())
		{
			std::string id1;
			MarshalString(object, id1);
			ZobObject* o = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id1);
			std::string id2;
			MarshalString(parent, id2);
			ZobObject* p = DirectZob::GetInstance()->GetZobObjectManager()->GetZobObjectFromlId(id2);
			if (o && p)
			{
				return GetInstance()->Reparent(o, p);
			}
		}
		return false;
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