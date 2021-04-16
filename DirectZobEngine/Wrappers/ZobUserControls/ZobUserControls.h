#ifdef _WINDLL
#pragma once
#include "../ManagedVector3.h"
#include "../../DirectZobEngine/Rendering/ZobVector3.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"
#include "../../Misc/ZobVariablesExposer.h"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;


namespace CLI {

	public ref class ZobGroupBox : GroupBox
	{
	public:
		ZobGroupBox(String^ name, bool collapsable);
		void OnToggle(Object^ sender, EventArgs^ e);
		bool bToggled;
	};

	public ref class ZobControlString : TableLayoutPanel
	{
	public:
		ZobControlString(ZobVariablesExposer::wrapperData* w);
		TextBox^ txt;
		delegate void OnChange(Object^ sender, EventArgs^ e);
		event OnChange^ OnChangeEvent;
	private:
		void OnValueChangedInternal(Object^ sender, EventArgs^ e);
	};

	public ref class ZobControlVector3 : TableLayoutPanel
	{
	public:
		ZobControlVector3(String^ name, String^ text, ZobVector3* v, bool readOnly, EventArgs^ OnChanged);
		void UpdateZobControl();
		void UpdateZobControlInternal();
		TextBox^ txt_X;
		TextBox^ txt_Y;
		TextBox^ txt_Z;
		ZobVector3* m_zobVector3;
		delegate void OnChange(Object^ sender, EventArgs^ e);
		event OnChange^ OnChangeEvent;
	private:
		void OnValueChangedInternal(Object^ sender, EventArgs^ e);
	};

	public ref class ZobControlTreeNode : public TreeNode
	{
	public:
		ZobControlTreeNode(String^ zobObjectGuid);
		ZobControlTreeNode^ GetChildNode(String^ guid);
		String^ m_zobObjectGuid;
	};

	public ref class ZobControlTreeview : TreeView
	{
	public:
		ZobControlTreeview();
		void UpdateZobControl();
	};

	public ref class ZobPropertiesContainer : TableLayoutPanel
	{
	public:
		ZobPropertiesContainer();
	};

	public ref class ZobPropertiesLayout : TableLayoutPanel
	{
	public:
		ZobPropertiesLayout(String^ name);
	};
};
#endif //_WINDLL