#ifdef _WINDLL
#pragma once
#include "../ManagedVector3.h"
#include "../../DirectZobEngine/Rendering/ZobVector3.h"
#include "../DirectZobWrapperEvents.h"
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

	public ref class ZobControl : TableLayoutPanel
	{
		public:
			ZobControl(const ZobVariablesExposer::wrapperData& w);
			~ZobControl();
	protected:
		virtual void	OnValueChanged(Object^ sender, EventArgs^ e) {}
		void			UpdateControl();
		virtual void	UpdateControlInternal() {}
		const ZobVariablesExposer::wrapperData* _w;
		DirectZobWrapperEvents::OnEditorUpdate^ _updateEvent;
	};

	public ref class ZobControlString : ZobControl
	{
	public:
		ZobControlString(const ZobVariablesExposer::wrapperData& w);
		TextBox^ txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
	};

	public ref class ZobControlFilePath : ZobControl
	{
	public:
		ZobControlFilePath(const ZobVariablesExposer::wrapperData& w);
		TextBox^ txt;
		Button^ btn;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void OnOpen(Object^ sender, EventArgs^ e);
		void OnOk(Object^ sender, System::ComponentModel::CancelEventArgs^ e);
	};

	public ref class ZobGroupBox : GroupBox
	{
	public:
		ZobGroupBox(String^ name, bool collapsable);
		void OnToggle(Object^ sender, EventArgs^ e);
		bool bToggled;
	};

	public ref class ZobControlVector3 : ZobControl
	{
	public:
		ZobControlVector3(const ZobVariablesExposer::wrapperData& w);
		void UpdateControlInternal() override;
		TextBox^ txt_X;
		TextBox^ txt_Y;
		TextBox^ txt_Z;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
	};

	public ref class ZobControlFloat : ZobControl
	{
	public:
		ZobControlFloat(const ZobVariablesExposer::wrapperData& w);
		TextBox^ txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlBool : ZobControl
	{
	public:
		ZobControlBool(const ZobVariablesExposer::wrapperData& w);
		CheckBox^ _checkBox;
		bool _checked;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlEnum : ZobControl
	{
	public:
		ZobControlEnum(const ZobVariablesExposer::wrapperData& w);
		ComboBox^ _combo;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlZobObject : ZobControl
	{
	public:
		ZobControlZobObject(const ZobVariablesExposer::wrapperData& w);
		TextBox^ txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlZobId : ZobControl
	{
	public:
		ZobControlZobId(const ZobVariablesExposer::wrapperData& w);
		Label^ txt;
	protected:
	};

	public ref class ZobControlTreeNode : public TreeNode
	{
	public:
		ZobControlTreeNode(String^ zobObjectGuid, bool isEditable);
		ZobControlTreeNode^ GetChildNode(String^ guid);
		bool IsEditable() { return m_isEditable; }
		String^ m_zobObjectGuid;
		bool m_isEditable;
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