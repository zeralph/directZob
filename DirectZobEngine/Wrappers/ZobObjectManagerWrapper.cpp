#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectManagerWrapper.h"
#include "../DirectZob.h"
#include "../ZobObjects/ZobObject.h"
#include "../Components/ZobComponentMesh.h"
#include "../Components/ZobComponentSprite.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{

	ZobObjectManagerWrapper::ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel, Resources::ResourceManager^ rsMgr):ManagedObject(DirectZob::GetInstance()->GetZobObjectManager(), false)
	{
		m_resourcesManager = rsMgr;
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
		m_duplicate = false;
	}

	System::String^ ZobObjectManagerWrapper::GetZobObjectList()
	{
		std::string s;
		m_Instance->GetZobObjectList(s);
		return gcnew System::String(s.c_str());
	}

	void ZobObjectManagerWrapper::EditorUpdate()
	{
		if (!m_treeView->IsHandleCreated)
			return;
		m_treeView->Invoke(gcnew Action(m_treeView, &CLI::ZobControlTreeview::UpdateZobControl));
		if (m_selectedObjectWrapper)
		{
			m_selectedObjectWrapper->EditorUpdate();
		}
		if (m_duplicate && m_selectedObjectWrapper)
		{
			m_selectedObjectWrapper->Duplicate();
			ReScan();
		}
		m_duplicate = false;
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
		ToolStripMenuItem^ itComponent = gcnew ToolStripMenuItem("Add Component");
		CreateComponentNodeMenu(itComponent);
		m_nodeMenu->Items->Add(itComponent);
		ToolStripMenuItem^ itZoom = gcnew ToolStripMenuItem("Zoom");
		itZoom->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::ZoomToZobObject);
		m_nodeMenu->Items->Add(itZoom);
		ToolStripMenuItem^ itDuplicate = gcnew ToolStripMenuItem("Duplicate");
		itDuplicate->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::DuplicateZobObject);
		m_nodeMenu->Items->Add(itDuplicate);
		//m_treeView->ContextMenuStrip = m_nodeMenu;
	}

	void ZobObjectManagerWrapper::CreateComponentNodeMenu(ToolStripMenuItem^ parent)
	{
		int l = ZobComponent::eComponentType::__eComponent_MAX__;
		for (int i = 0; i < l; i++)
		{
			String^ s = gcnew String(ZobComponentFactory::eComponentTypeStr[i]);
			ToolStripItem^ it = gcnew ToolStripMenuItem(s);
			it->Click += gcnew EventHandler(this, &ZobObjectManagerWrapper::AddZobComponent);
			parent->DropDownItems->Add(it);
		}
	}

	void ZobObjectManagerWrapper::AddZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		zobId zid = ZobEntity::ZobIdFromString(id);
		ZobObject* z = ZobEntity::GetEntity<ZobObject>(zid);
		DirectZob::GetInstance()->GetZobObjectManager()->CreateZobObject(z);
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
	}

	void ZobObjectManagerWrapper::RemoveZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		zobId zid = ZobEntity::ZobIdFromString(id);
		ZobObject* p = ZobEntity::GetEntity<ZobObject>(zid);
		if (m_selectedObject == p)
		{
			SelectObject(p->GetParent());
		}
		DirectZob::GetInstance()->GetZobObjectManager()->RemoveZobObject(p);
		ReScan((ZobControlTreeNode^)m_treeView->TopNode, m_bShowAllNodes);
	}
	void ZobObjectManagerWrapper::DuplicateZobObject(Object^ sender, EventArgs^ e)
	{
		m_duplicate = true;
	}
	void ZobObjectManagerWrapper::ZoomToZobObject(Object^ sender, EventArgs^ e)
	{
		String^ guid = ((ZobControlTreeNode^)m_treeView->SelectedNode)->m_zobObjectGuid;
		std::string id;
		MarshalString(guid, id);
		zobId zid = ZobEntity::ZobIdFromString(id);
		ZobObject* p = ZobEntity::GetEntity<ZobObject>(zid);
		Camera* c = DirectZob::GetInstance()->GetCameraManager()->GetCamera(std::string("EditorCamera"));
		if (c && p)
		{
			ZobVector3 pos = p->GetWorldPosition();
			c->SetTarget(&pos);
		}
	}
	void ZobObjectManagerWrapper::AddZobComponent(Object^ sender, EventArgs^ e)
	{
		if (m_selectedObject)
		{
			ToolStripMenuItem^ it = (ToolStripMenuItem^)sender;
			int l = ZobComponent::eComponentType::__eComponent_MAX__;
			for (int i = 0; i < l; i++)
			{
				String^ s = gcnew String(ZobComponentFactory::eComponentTypeStr[i]);
				if (s == it->Text)
				{
					ZobComponentFactory::CreateComponent(m_selectedObject, ZobComponentFactory::eComponentTypeStr[i], false);
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
		m_treeView->ImageList = gcnew ImageList();
		
		Image^ im = (Image^)m_resourcesManager->GetObject("_object");
		if (im != nullptr)
		{
			Bitmap^ bmp = gcnew Bitmap(im);
			m_treeView->ImageList->Images->Add(bmp);
		}
		im = (Image^)m_resourcesManager->GetObject("light1");
		if (im != nullptr)
		{
			Bitmap^ bmp = gcnew Bitmap(im);
			m_treeView->ImageList->Images->Add(bmp);
		}
		im = (Image^)m_resourcesManager->GetObject("camera2");
		if (im != nullptr)
		{
			Bitmap^ bmp = gcnew Bitmap(im);
			m_treeView->ImageList->Images->Add(bmp);
		}
		

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
			if (z && !z->IsSelectable())
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
			if (tn->IsSelectable())
			{
				m_treeView->SelectedNode = e->Node;
				if (e->Button == MouseButtons::Left)
				{
					SelectObject(GetZobObject(tn->m_zobObjectGuid));
				}
				else if (e->Button == MouseButtons::Right)
				{
					if (SelectObject(GetZobObject(tn->m_zobObjectGuid)))
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
		m_treeView->Invoke(gcnew Action(this, &CLI::ZobObjectManagerWrapper::ReScanInternal));
	}

	void ZobObjectManagerWrapper::ReScanInternal()
	{
		ZobControlTreeNode^ t = (ZobControlTreeNode^)m_treeView->TopNode;
		bool showAllNodes = false;
		ReScan(t, showAllNodes);
	}

	void ZobObjectManagerWrapper::ReScan(ZobControlTreeNode^ n, bool showAllNodes)
	{
		if (!n->IsSelectable())
		{
			return;
		}
		ZobObject* z = GetZobObject(n->m_zobObjectGuid);
		if (z && !z->IsMarkedForDeletion())
		{
			if (m_selectedObject == z)
			{
				m_treeView->SelectedNode = n;
				//n->SelectedImageIndex()
			}
			const std::vector<ZobObject*>* v = z->GetChildren();
			//const ZobComponentMesh* test = z->GetComponent<ZobComponentMesh>();
			List<String^>^ l = gcnew List<String^>();
			for (int i = 0; i < v->size(); i++)
			{
				ZobObject* child = v->at(i);
				if (!child->IsMarkedForDeletion())
				{
					String^ guid = TO_MANAGED_STRING(child->ZobIdToString().c_str());
					bool isEditor = ZobEntity::GetType(child->GetIdValue()) == ZobEntity::type_editor;
					if (!isEditor)
					{
						l->Add(guid);
						ZobControlTreeNode^ childNode = n->GetChildNode(guid);
						if (!childNode)
						{
							ZobControlTreeNode^ newNode = gcnew ZobControlTreeNode(guid);
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
			String^ guidStr = TO_MANAGED_STRING(z->ZobIdToString().c_str());
			bool isEditor = ZobEntity::GetType(z->GetIdValue()) == ZobEntity::type_editor;
			if (!isEditor)
			{
				ZobControlTreeNode^ tn = gcnew ZobControlTreeNode(guidStr);
				const char* nameStr = z->GetName().c_str();
				tn->Text = TO_MANAGED_STRING(nameStr);
				collection->Add(tn);
				for (int i = 0; i < z->GetChildren()->size(); i++)
				{
					AddZobObjectsRecursive(z->GetChild(i), tn->Nodes);
				}
			}
		}
	}

	ZobObject* ZobObjectManagerWrapper::GetZobObject(System::String^ guid)
	{
		std::string id;
		MarshalString(guid, id);
		zobId zid = ZobEntity::ZobIdFromString(id);
		ZobObject* z = ZobEntity::GetEntity<ZobObject>(zid);
		return z;
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

	bool ZobObjectManagerWrapper::SelectObject(ZobObject* z)
	{
		if (m_selectedObjectWrapper)
		{
			//m_selectedObjectWrapper->Detach();
			delete m_selectedObjectWrapper; 
			m_selectedObject = NULL;
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
			return true;
		}
		else
		{
			if (m_selectedObjectWrapper)
			{
				delete m_selectedObjectWrapper;
			}
			m_selectedObject = NULL;			
			OnObjectSelectedEvent(nullptr);
			return false;
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
			zobId zid = ZobEntity::ZobIdFromString(id1);
			ZobObject* o = ZobEntity::GetEntity<ZobObject>(zid);
			std::string id2;
			MarshalString(parent, id2);
			zobId zid2 = ZobEntity::ZobIdFromString(id2);
			ZobObject* p = ZobEntity::GetEntity<ZobObject>(zid2);
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