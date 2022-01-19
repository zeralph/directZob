#ifdef _WINDLL
#pragma once
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

	public ref class ZobComboboxItem
	{
	public:
		ZobComboboxItem(int k, String^ v, const ZobVariablesExposer::wrapperData& w)
		{
			Key = k;
			Value = v;
			_w = &(w);
		}
		String^ ToString() override
		{
			return Value;
		}
		int Key;
		String^ Value;
		const ZobVariablesExposer::wrapperData* _w;
	};

	public ref class ZobTextBox : TextBox
	{
	public:
		ZobTextBox();
		~ZobTextBox();
	};

	public ref class ZobButton : Button
	{
	public:
		ZobButton();
		~ZobButton();
	};

	public ref class ZobLabel : Label
	{
	public:
		ZobLabel();
		~ZobLabel();
	};

	public ref class ZobGroupBox : GroupBox
	{
	public:
		ZobGroupBox(String^ name, Image^ im, bool collapsable);
		~ZobGroupBox();
		void OnToggle(Object^ sender, MouseEventArgs^ e);
		void OnToggle2(Object^ sender, EventArgs^ e);
		bool bToggled;
		PictureBox^ _picBox;
		Image^ _image;
		MouseEventHandler^ _event;
		EventHandler^ _event2;
	};

	public ref class ZobControl : TableLayoutPanel
	{
		public:
			ZobControl(const ZobVariablesExposer::wrapperData& w);
			~ZobControl();
			static ZobGroupBox^ CreateWrappedVariablesView(std::string& name, ZobVariablesExposer* ze, Image^ im);
			static const ZobVariablesExposer::wrapperData* GetDataFromWrapperVariable(String^ variableName, ZobVariablesExposer* ze);
	protected:
		virtual void	OnValueChanged(Object^ sender, EventArgs^ e) {}
		void			UpdateControl();
		virtual void	UpdateControlInternal() {}
		bool			IsControlOk();
		const ZobVariablesExposer::wrapperData* _w;
		DirectZobWrapperEvents::OnEditorUpdate^ _updateEvent;
		Action^ _update;
	};

	public ref class ZobControlString : ZobControl
	{
	public:
		ZobControlString(const ZobVariablesExposer::wrapperData& w);
		~ZobControlString();
		EventHandler^ _event;
		ZobLabel^ _label;
		ZobTextBox^ _txt;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
	};

	public ref class ZobControlFilePath : ZobControl
	{
	public:
		ZobControlFilePath(const ZobVariablesExposer::wrapperData& w);
		~ZobControlFilePath();
		ZobTextBox^ _txt;
		ZobLabel^ _label;
		ZobButton^ _btn;
		ZobButton^ _rst;
		EventHandler^ _eventSet;
		EventHandler^ _eventRst;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void OnOpen(Object^ sender, EventArgs^ e);
		void OnReset(Object^ sender, EventArgs^ e);
		void OnOk(Object^ sender, System::ComponentModel::CancelEventArgs^ e);
	};

	public ref class ZobControlVector3 : ZobControl
	{
	public:
		ZobControlVector3(const ZobVariablesExposer::wrapperData& w);
		~ZobControlVector3();
		void UpdateControlInternal() override;
		ZobTextBox^ _txt_X;
		ZobTextBox^ _txt_Y;
		ZobTextBox^ _txt_Z;
		ZobLabel^ _label;
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
		ZobTextBox^ _txt;
		ZobLabel^ _label;
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
		ZobLabel^ _label;
		CheckBox^ _checkBox;
		bool _checked;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlAction : ZobControl
	{
	public:
		ZobControlAction(const ZobVariablesExposer::wrapperData& w);
		~ZobControlAction();
		EventHandler^ _event;
		ZobLabel^ _label;
		ZobButton^ _button;
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
		ZobTextBox^ _txt_A;
		ZobTextBox^ _txt_R;
		ZobTextBox^ _txt_G;
		ZobTextBox^ _txt_B;
		ZobLabel^ _label;
		EventHandler^ _eventA;
		EventHandler^ _eventR;
		EventHandler^ _eventG;
		EventHandler^ _eventB;
		ZobButton^ _btn;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void OnClickColor(Object^ sender, EventArgs^ e);
		void UpdateControlInternal() override;
	};

	public ref class ZobControlEnumCombo : ZobControl
	{
	public:
		ZobControlEnumCombo(const ZobVariablesExposer::wrapperData& w);
		~ZobControlEnumCombo();
		ZobLabel^ _label;
		CheckedListBox^ _list;
		EventHandler^ _event;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlEnum : ZobControl
	{
	public:
		ZobControlEnum(const ZobVariablesExposer::wrapperData& w);
		~ZobControlEnum();
		ZobLabel^ _label;
		ComboBox^ _list;
		EventHandler^ _event;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
	};

	public ref class ZobControlZobEntity : ZobControl
	{
	public:
		ZobControlZobEntity(const ZobVariablesExposer::wrapperData& w);
		~ZobControlZobEntity();
		ZobLabel^ _label;
		ZobTextBox^ _txt;
		ZobButton^ _btn;
	protected:
		void OnValueChanged(Object^ sender, EventArgs^ e) override;
		void UpdateControlInternal() override;
		void OnClick(Object^ sender, EventArgs^ e);
	};

	public ref class ZobControlZobId : ZobControl
	{
	public:
		ZobControlZobId(const ZobVariablesExposer::wrapperData& w);
		~ZobControlZobId();
		ZobLabel^ _label;
		ZobLabel^ _txt;
	protected:
	};

	public ref class ZobControlTreeNode : public TreeNode
	{
	public:
		ZobControlTreeNode(String^ zobObjectGuid);
		~ZobControlTreeNode();
		ZobControlTreeNode^ GetChildNode(String^ guid);
		bool IsSelectable() { return m_isSelectable; }
		bool IsReadOnly() { return m_isReadOnly; }
		String^ m_zobObjectGuid;
		bool m_visible;
		bool m_locked;
		EventHandler^ _event;
		bool m_isSelectable;
		bool m_isReadOnly;
		void OnClick(Object^ sender, TreeNodeMouseClickEventArgs^ e);
	};

	public ref class ZobControlTreeview : TreeView
	{
	public:
		ZobControlTreeview();
		void UpdateZobControl();
	protected:
		void OnDrawNode(DrawTreeNodeEventArgs^ e) override;
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
		ZobLabel^ _label;
	};
};
#endif //_WINDLL