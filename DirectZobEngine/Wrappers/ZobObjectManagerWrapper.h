#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Types.h"
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
	public ref class ZobEntityInfo
	{
	public:
		enum class eImageObjectType
		{
			eImageZobObject = 0,
			eImageZobLight = 1,
			eImageZobCamera = 2,
			eImageZobMesh = 3,
			eImageZobSprite = 4,
			eImageZobHud = 5,
			eImageZobComponent = 6,
			eImageZobCameraController = 7,
			eImageZobunknown = 8,
			__eImage_MAX__ = 6,
		};
		 
		static const cli::array<System::String^>^ eImageObjectTypeStr = { "_object", "light1", "camera2", "mesh3", "sprite", "hud", "component", "cameraController", "unknown"};

		ZobEntityInfo(ZobEntity* ze);
		ZobEntityInfo(DirectZobType::zobId zid);
		String^ _name;
		ulong _zid;
		String^ _type;
		bool _isSelectable;
		bool _isReadOnly;
		bool _isEditor;
		Color _color;
		eImageObjectType _imgType;
		String^ _imgName;
	private:
		void Init(ZobEntity* ze);
	};

	public ref class ZobObjectManagerWrapper : public ManagedObject<ZobObjectManager>
	{
	public:

		delegate void					ZobComponentHandler(int i);

		delegate void					OnObjectSelected(ZobObjectWrapper^ z);
		static event OnObjectSelected^	OnObjectSelectedEvent;

		enum class eObjectTypes
		{
			eObjectTypes_all = 0,
			eObjectTypes_editor,
			eObjectTypes_scene,
		};

		ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel, Resources::ResourceManager^ rsMgr);
		void					SaveTransforms();
		void					RestoreTransforms();
		void					Refresh();
		System::String^			GetZobObjectList();
		ZobObject*				GetZobObject(System::String^ guid);
		ZobObjectWrapper^		GetRootObject();
		void					RemoveZobObject(ZobObjectWrapper^ name);
		ZobObjectWrapper^		CreateZobObject();
		ZobObjectWrapper^		AddZobSprite(ZobObjectWrapper^ parent);
		bool					Reparent(String^ object, String^ parent);
		void					AddZobObjectsRecursive(ZobObject* z, TreeNodeCollection^ collection);
		void					EditorUpdate();
		void					AddEditorGizmos();
		TreeView^				GetTreeviw() {return m_treeView;}
		void					ReScan();
		bool					SelectObject(ZobObject* z);
		ZobObject*				GetSelectedObject() {return m_selectedObject;}
		ZobObjectsEditor*		GetEditorGizmos() { return m_editorGizmos; }
		List<ZobEntityInfo^>^	GetEntitiesInfo();
	private:
	
		void					CreateTreeview();
		void					CreateObjectview();
		void					CreateNodeMenu();
		void					CreateComponentNodeMenu(ToolStripMenuItem^ parent);
		void					ReScanInternal();
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
		void					AddZobComponent(Object^ sender, EventArgs^ e);
		void					DuplicateZobObject(Object^ sender, EventArgs^ e);

		Panel^ m_objectTreeviewPanel;
		Panel^ m_objectPropertiesPanel;
		ZobObjectsEditor* m_editorGizmos;
		CLI::ZobControlTreeview^ m_treeView;
		ZobObject* m_selectedObject;
		ZobObjectWrapper^ m_selectedObjectWrapper;
		ZobControlTreeNode^ m_draggedNode;
		ContextMenuStrip^ m_nodeMenu;
		Resources::ResourceManager^ m_resourcesManager;
		bool m_bShowAllNodes;
		bool					m_duplicate;
	};
}
#endif //_WINDLL