#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Managers/ZobObjectManager.h"
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
	public ref class ZobObjectManagerTreeNode : public TreeNode
	{
	public:
		String^ m_zobObjectGuid;
	};

	public ref class ZobObjectManagerWrapper : public ManagedObject<ZobObjectManager>
	{
	public:
		enum class eObjectTypes
		{
			eObjectTypes_all = 0,
			eObjectTypes_editor,
			eObjectTypes_scene,
		};

		ZobObjectManagerWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel);
		void SaveTransforms();
		void RestoreTransforms();
		void Refresh();
		System::String^ GetZobObjectList();
		ZobObjectWrapper^ GetZobObject(System::String^ name);
		ZobObjectWrapper^ GetRootObject();
		void RemoveZobObject(ZobObjectWrapper^ name);
		ZobObjectWrapper^ AddZobObject(ZobObjectWrapper^ parent);
		ZobObjectWrapper^ AddZobSprite(ZobObjectWrapper^ parent);
		ZobObjectWrapper^ GetObjectAtCoords(int x, int y, eObjectTypes type);
		ZobObject* GetSelectedObject() {return m_selectedObject;}
		void CreateEditorGizmos(System::String^ editorResourcesPath);
		bool Reparent(ZobObjectWrapper^ o, ZobObjectWrapper^ parent);
		void AddZobObjectsRecursive(ZobObject* z, TreeNodeCollection^ collection);
		void OnEditorUpdate(Object^ sender, EventArgs^ e);
	private:

		
		void CreateTreeview();
		void CreateObjectview();

		void TreeNodeClick(Object^ sender, TreeViewEventArgs^ e);

		Panel^ m_objectTreeviewPanel;
		Panel^ m_objectPropertiesPanel;

		CLI::ZobControlTreeview^ m_treeView;
		ZobObject* m_selectedObject;
		ZobObjectWrapper^ m_selectedObjectWrapper;
	};
}
#endif //_WINDLL