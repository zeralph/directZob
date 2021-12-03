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
		~ZobControlString();
		EventHandler^ _event;
		TextBox^ _txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
	};

	public ref class ZobControlFilePath : ZobControl
	{
	public:
		ZobControlFilePath(const ZobVariablesExposer::wrapperData& w);
		~ZobControlFilePath();
		TextBox^ _txt;
		Label^ _label;
		Button^ _btn;
		EventHandler^ _event;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void OnOpen(Object^ sender, EventArgs^ e);
		void OnOk(Object^ sender, System::ComponentModel::CancelEventArgs^ e);
	};

	public ref class ZobGroupBox : GroupBox
	{
	public:
		ZobGroupBox(String^ name, bool collapsable);
		~ZobGroupBox();
		void OnToggle(Object^ sender, EventArgs^ e);
		bool bToggled;
		Label^ _label;
		EventHandler^ _event;
	};

	public ref class ZobControlVector3 : ZobControl
	{
	public:
		ZobControlVector3(const ZobVariablesExposer::wrapperData& w);
		~ZobControlVector3();
		void UpdateControlInternal() override;
		TextBox^ _txt_X;
		TextBox^ _txt_Y;
		TextBox^ _txt_Z;
		Label^ _label;
		EventHandler^ _eventX;
		EventHandler^ _eventY;
		EventHandler^ _eventZ;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
	};

	public ref class ZobControlFloat : ZobControl
	{
	public:
		ZobControlFloat(const ZobVariablesExposer::wrapperData& w);
		~ZobControlFloat();
		TextBox^ _txt;
		Label^ _label;
		EventHandler^ _event;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlBool : ZobControl
	{
	public:
		ZobControlBool(const ZobVariablesExposer::wrapperData& w);
		~ZobControlBool();
		EventHandler^ _event;
		Label^ _label;
		CheckBox^ _checkBox;
		bool _checked;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlColor : ZobControl
	{
	public:
		ZobControlColor(const ZobVariablesExposer::wrapperData& w);
		~ZobControlColor();
		TextBox^ _txt_A;
		TextBox^ _txt_R;
		TextBox^ _txt_G;
		TextBox^ _txt_B;
		Label^ _label;
		EventHandler^ _eventA;
		EventHandler^ _eventR;
		EventHandler^ _eventG;
		EventHandler^ _eventB;
		Button^ _btn;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void OnClickColor(Object^ sender, EventArgs^ e);
		void UpdateControlInternal() override;
	};

	public ref class ZobControlEnum : ZobControl
	{
	public:
		ZobControlEnum(const ZobVariablesExposer::wrapperData& w);
		~ZobControlEnum();
		Label^ _label;
		ComboBox^ _list;
		EventHandler^ _event;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlZobObject : ZobControl
	{
	public:
		ZobControlZobObject(const ZobVariablesExposer::wrapperData& w);
		~ZobControlZobObject();
		Label^ _label;
		TextBox^ _txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlZobId : ZobControl
	{
	public:
		ZobControlZobId(const ZobVariablesExposer::wrapperData& w);
		~ZobControlZobId();
		Label^ _label;
		Label^ _txt;
	protected:
	};

	public ref class ZobControlTreeNode : public TreeNode
	{
	public:
		ZobControlTreeNode(String^ zobObjectGuid, bool isEditable);
		~ZobControlTreeNode();
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
		~ZobPropertiesLayout();
		Label^ _label;
	};
};
#endif //_WINDLL