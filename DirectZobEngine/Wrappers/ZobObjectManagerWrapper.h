#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Managers/ZobObjectManager.h"
#include "Editor/ZobObjectsEditor.h"
#include "ZobObjectWrapper.h"
#include <string> 
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace CLI
{
	public ref class ZobObjectManagerWrapper : public ManagedObject<ZobObjectManager>
	{
	public:

		delegate void					ZobBehaviorHandler(int i);

		delegate void					OnObjectSelected(ZobObjectWrapper^ z);
		static event OnObjectSelected^	OnObjectSelectedEvent;

		enum class eObjectTypes
		{
			eObjectTypes_all = 0,
			eObjectTypes_editor,
			eObjectTypes_scene,
		};

		ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel);
		void					SaveTransforms();
		void					RestoreTransforms();
		void					Refresh();
		System::String^			GetZobObjectList();
		ZobObject*				GetZobObject(System::String^ guid);
		ZobObjectWrapper^		GetRootObject();
		void					RemoveZobObject(ZobObjectWrapper^ name);
		ZobObjectWrapper^		CreateZobObject();
		ZobObjectWrapper^		AddZobSprite(ZobObjectWrapper^ parent);
		ZobObjectWrapper^		GetObjectAtCoords(int x, int y, eObjectTypes type);
		ZobObjectWrapper^		GetSelectedObject() {return m_selectedObjectWrapper;}
		bool					Reparent(String^ object, String^ parent);
		void					AddZobObjectsRecursive(ZobObject* z, TreeNodeCollection^ collection);
		void					EditorUpdate();
		void					AddEditorGizmos();
		TreeView^				GetTreeviw() {return m_treeView;}
		void					ReScan();
		void					SelectObject(ZobObject* z);
		ZobObjectsEditor*		GetEditorGizmos() { return m_editorGizmos; }
	private:
	
		void					CreateTreeview();
		void					CreateObjectview();
		void					CreateNodeMenu();
		void					CreateBehaviorNodeMenu(ToolStripMenuItem^ parent);
		void					ReScan(ZobControlTreeNode^ n, bool showAllNodes);
		void					OnNewScene();
		void					TreeNodeMouseHover(Object^ sender, EventArgs^ e);
		void					TreeNodeClick(Object^ sender, TreeNodeMouseClickEventArgs^ e);
		void					ItemDrag(Object^ sender, ItemDragEventArgs^ e);
		void					DragDrop(Object^ sender, DragEventArgs^ e);
		void					DragEnter(Object^ sender, DragEventArgs^ e);
		void					DragOver(Object^ sender, DragEventArgs^ e);
		void					DragLeave(Object^ sender, EventArgs^ e);
		void					TreeNodeBeforeSelect(Object^ sender, TreeViewCancelEventArgs^ e);
		void					AddZobObject(Object^ sender, EventArgs^ e);
		void					RemoveZobObject(Object^ sender, EventArgs^ e);
		void					ZoomToZobObject(Object^ sender, EventArgs^ e);
		void					AddZobBehavior(Object^ sender, EventArgs^ e);

		Panel^ m_objectTreeviewPanel;
		Panel^ m_objectPropertiesPanel;
		ZobObjectsEditor* m_editorGizmos;
		CLI::ZobControlTreeview^ m_treeView;
		ZobObject* m_selectedObject;
		ZobObjectWrapper^ m_selectedObjectWrapper;
		ZobControlTreeNode^ m_draggedNode;
		ContextMenuStrip^ m_nodeMenu;
		bool m_bShowAllNodes;
	};
}
#endif //_WINDLL