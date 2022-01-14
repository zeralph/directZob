#include "ZobUserControls.h"
#include "../DirectZobWrapper.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

using namespace CLI;
static ContentAlignment _alignment = ContentAlignment::MiddleLeft;
static int _height = 15;
static int _labelWidth = 100;
static int _fontSize = 8;
static int _btnFontSize = 6;
static char* _fontName = "Tahoma";



static void MarshalString(System::String^ s, std::string& os) {
	using namespace System::Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

ZobControl::ZobControl(const ZobVariablesExposer::wrapperData& w) :TableLayoutPanel()
{
	_w = &w;
	_updateEvent = gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControl::UpdateControl);
	DirectZobWrapperEvents::OnEditorUpdateEvent += _updateEvent;
	_update = gcnew Action(this, &ZobControl::UpdateControlInternal);
	static int yy = 10;
	this->RowStyles->Add(gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::AutoSize, yy));
}

ZobTextBox::ZobTextBox()
{
	System::Drawing::Font^ f = gcnew System::Drawing::Font("Tahoma", _fontSize);
	this->Font = f;
	this->Top = this->Height - (this->Font->Height / 2 + 1);
	this->Margin.All = 0;
	this->Padding.All = 0;
}

ZobTextBox::~ZobTextBox()
{

}

ZobLabel::ZobLabel()
{
	System::Drawing::Font^ f = gcnew System::Drawing::Font("Tahoma", _fontSize);
	this->Top = this->Height - (this->Font->Height / 2 + 1);
	this->Margin.All = 0;
	this->Padding.All = 0;
}

ZobLabel::~ZobLabel()
{

}

ZobButton::ZobButton()
{
	System::Drawing::Font^ f = gcnew System::Drawing::Font("Tahoma", _btnFontSize);
	this->Font = f;
	this->Margin.All = 0;
	this->Padding.All = 0;
}

ZobButton::~ZobButton()
{

}

ZobControl::~ZobControl()
{
	//delete _update;
	for (int i = 0; i < this->Controls->Count; i++)
	{
		//delete(this->Controls[i]);
	}
	_update = nullptr;
	DirectZobWrapperEvents::OnEditorUpdateEvent -= _updateEvent;
	//delete _updateEvent;
	_updateEvent = nullptr;
	_w = NULL;
}

bool ZobControl::IsControlOk()
{
	return this && this->IsHandleCreated && !this->IsDisposed && !this->Disposing;
}

void ZobControl::UpdateControl() 
{ 
	try
	{
		if (IsControlOk())
		{
			this->Invoke(_update);
		}
	}
	catch (System::ObjectDisposedException^ e)
	{
		DirectZob::LogWarning("ERROR : %s", e->ToString());
	}
}

ZobGroupBox^ ZobControl::CreateWrappedVariablesView(std::string& name, ZobVariablesExposer* ze, Image^ im)
{
	String^ panelName = TO_MANAGED_STRING(name.c_str());
	ZobGroupBox^ zobPanel = gcnew ZobGroupBox(panelName, im, true);
	const std::vector<ZobVariablesExposer::wrapperData>* v = ze->GetWrappedVariables();
	if (v->size() > 0)
	{
		ZobPropertiesContainer^ panel = gcnew ZobPropertiesContainer();
		int idx = 1;
		for (std::vector<ZobVariablesExposer::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
		{
			const ZobVariablesExposer::wrapperData& w = (*iter);
			if (w.type == ZobVariablesExposer::eWrapperType_path)
			{
				panel->Controls->Add(gcnew ZobControlFilePath(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_string)
			{
				panel->Controls->Add(gcnew ZobControlString(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_ZobVector3)
			{
				panel->Controls->Add(gcnew ZobControlVector3(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_float)
			{
				panel->Controls->Add(gcnew ZobControlFloat(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_enum)
			{
				panel->Controls->Add(gcnew ZobControlEnum(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_enumCombo)
			{
				panel->Controls->Add(gcnew ZobControlEnumCombo(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_bool)
			{
				panel->Controls->Add(gcnew ZobControlBool(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_int)
			{
				panel->Controls->Add(gcnew ZobControlFloat(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_startGroup)
			{
				System::String^ s = gcnew String(w.name.c_str());
				ZobGroupBox^ zb = gcnew ZobGroupBox(s, nullptr, false);
				ZobPropertiesContainer^ p = gcnew ZobPropertiesContainer();
				zb->Controls->Add(p);
				panel->Controls->Add(zb);
				panel = p;
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_endGroup)
			{
				ZobPropertiesContainer^ p = (ZobPropertiesContainer^)panel->Parent->Parent;
				panel = p;
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_zobId)
			{
				panel->Controls->Add(gcnew ZobControlZobId(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_zobEntity)
			{
				panel->Controls->Add(gcnew ZobControlZobEntity(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_zobColor)
			{
				panel->Controls->Add(gcnew ZobControlColor(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_uint)
			{
				panel->Controls->Add(gcnew ZobControlFloat(w));
			}
			else if (w.type == ZobVariablesExposer::eWrapperType_action)
			{
				panel->Controls->Add(gcnew ZobControlAction(w));
			}
			else
			{
				assert(NULL);
			}

			idx++;
		}
		zobPanel->Controls->Add(panel);
	}
	return zobPanel;
}

const ZobVariablesExposer::wrapperData* ZobControl::GetDataFromWrapperVariable(String^ variableName, ZobVariablesExposer* ze)
{
	const std::vector<ZobVariablesExposer::wrapperData>* v = ze->GetWrappedVariables();
	if (v->size() > 0)
	{
		for (std::vector<ZobVariablesExposer::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
		{
			const ZobVariablesExposer::wrapperData* w = &(*iter);
			String^ wName = TO_MANAGED_STRING(w->internalName.c_str());
			if (wName == variableName)
			{
				return w;
			}
		}
	}
	return NULL;
}

ZobControlString::ZobControlString(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->Name = TO_MANAGED_STRING(w.internalName.c_str());
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->AutoSize = false;
	_label->TextAlign = _alignment;
	_txt = gcnew ZobTextBox();
	_txt->AutoSize = false;
	_txt->Width = 140;
	_txt->Height = _height * 2;
	_txt->ReadOnly = (bool)w.bReadOnly;
	_txt->Multiline = true;
	std::string* s = (std::string*)(w.ptr);
	_txt->Text = TO_MANAGED_STRING(s->c_str());
	if (!_txt->ReadOnly)
	{
		_event = gcnew EventHandler(this, &ZobControlString::OnValueChanged);;
		_txt->Leave += _event;
	}
	this->Controls->Add(_label, 0, 0);
	this->Controls->Add(_txt, 1, 0);
}
ZobControlString::~ZobControlString()
{
	_txt->Leave -= _event;
	//delete _label;
	//delete _event;
	//delete _txt;
}

void ZobControlString::OnValueChanged(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlString^ zb = (ZobControlString^)tb->Parent;
	if (zb->_w)
	{
		std::string* internalString = (std::string*)_w->ptr;
		std::string s;
		MarshalString(tb->Text, s);
		internalString->assign(s);
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

ZobControlFilePath::ZobControlFilePath(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 4;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	//label->Dock = DockStyle::Fill;
	_label->Width = 100;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_txt = gcnew ZobTextBox();
	_txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_txt->AutoSize = false;
	_txt->Width = 140;
	_txt->Height = _height;
	_txt->ReadOnly = true;
	_btn = gcnew ZobButton();
	_btn->AutoSize = false;
	_btn->Width = 20;
	_btn->Height = _height;
	_btn->Text = "+";
	_rst = gcnew ZobButton();
	_rst->AutoSize = false;
	_rst->Width = 20;
	_rst->Height = _height;
	_rst->Text = "-";
	//txt->MaximumSize = Drawing::Size(100, 20);
	std::string s;
	ZobFilePath* zp = (ZobFilePath*)(w.ptr);
	_txt->Text = TO_MANAGED_STRING(zp->GetName().c_str());
	_txt->ReadOnly = true;
	if (!w.bReadOnly)
	{
		//txt->Leave += gcnew EventHandler(this, &ZobControlFilePath::OnValueChanged);
		_eventSet = gcnew EventHandler(this, &ZobControlFilePath::OnOpen);
		_btn->Click += _eventSet;
		_eventRst = gcnew EventHandler(this, &ZobControlFilePath::OnReset);
		_rst->Click += _eventRst;
	}
	this->Controls->Add(_label);
	this->Controls->Add(_txt);
	this->Controls->Add(_btn);
	this->Controls->Add(_rst);
}

ZobControlFilePath::~ZobControlFilePath()
{
	_btn->Click -= _eventSet;
	_rst->Click -= _eventRst;
	//delete _btn;
	//delete _label;
	//delete _txt;
	//delete _event;
}

void ZobControlFilePath::OnReset(Object^ sender, EventArgs^ e)
{
	ZobFilePath* zp = (ZobFilePath*)_w->ptr;
	_txt->Text = "";
	zp->Reset();
}

void ZobControlFilePath::OnOpen(Object^ sender, EventArgs^ e)
{
	OpenFileDialog^ f = gcnew OpenFileDialog();
	ZobFilePath* zp = (ZobFilePath*)_w->ptr;
	switch (zp->GetFileType())
	{
	case ZobFilePath::eFileType_all:
		f->Filter = "All files (*.*)|*.*";
		break;
	case ZobFilePath::eFileType_mesh:
		f->Filter = "Mesh files (*.obj, *.fbx)|*.obj;*.fbx";
		break;
	case ZobFilePath::eFileType_texture:
		f->Filter = "Image files (*.png, *.tga, *.jpg, *.jpeg)|*.png;*.tga;*.jpeg;*.jpg";
		break;
	case ZobFilePath::eFileType_xml:
		f->Filter = "Xml files (*.xml)|*.xml";
		break;
	}
	f->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ZobControlFilePath::OnOk);
	f->ShowDialog();
}

void ZobControlFilePath::OnOk(Object^ sender, System::ComponentModel::CancelEventArgs^ e)
{
	OpenFileDialog^ f = (OpenFileDialog^)sender;
	String^ fullPath = f->FileName;
	System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(fullPath);
	String^ dir = fi->DirectoryName;
	dir = dir->Replace('\\', '/');
	if (!dir->EndsWith("/"))
	{
		dir += "/";
	}
	String^ file = fi->Name;
	_txt->Text = file;
	if (_w)
	{
		std::string s;
		MarshalString(file+";"+dir+";"+file+";1", s);
		ZobFilePath* zp = (ZobFilePath*)_w->ptr;
		zp->Unserialize(s);
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

void ZobControlFilePath::OnValueChanged(Object^ sender, EventArgs^ e)
{
}


ZobControlVector3::ZobControlVector3(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	String^ internalName = TO_MANAGED_STRING(w.internalName.c_str());
	String^ name = TO_MANAGED_STRING(w.name.c_str());
	ZobVector3* v = (ZobVector3*)(w.ptr);
	this->AutoSize = true;
	this->ColumnCount = 4;
	this->RowCount = 1;
	this->Name = name;
	int width = 80;
	_label = gcnew ZobLabel();
	_label->Text = name;
	_label->Width = width;
	_label->Height = _height;
	_label->AutoSize = false;
	_label->TextAlign = _alignment;
	_label->Width = _labelWidth;
	_txt_X = gcnew ZobTextBox();
	_txt_X->AutoSize = false;
	_txt_X->Name = name;
	_txt_X->Width = width;
	_txt_X->Height = _height;
	_txt_X->ReadOnly = _w->bReadOnly;

	_txt_Y = gcnew ZobTextBox();
	_txt_Y->AutoSize = false;
	_txt_Y->Name = name;
	_txt_Y->Width = width;
	_txt_Y->Height = _height;
	_txt_Y->ReadOnly = _w->bReadOnly;

	_txt_Z = gcnew ZobTextBox();
	_txt_Z->Name = name;
	_txt_Z->Width = width;
	_txt_Z->AutoSize = false;
	_txt_Z->Height = _height;
	_txt_X->AutoSize = false;
	_txt_Z->ReadOnly = _w->bReadOnly;

	UpdateControlInternal();
	_eventX = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	_eventY = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	_eventZ = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	_txt_X->Leave += _eventX;
	_txt_Y->Leave += _eventY;
	_txt_Z->Leave += _eventZ;

	this->Controls->Add(_label, 0, 0);
	this->Controls->Add(_txt_X, 1, 0);
	this->Controls->Add(_txt_Y, 2, 0);
	this->Controls->Add(_txt_Z, 3, 0);

}

ZobControlVector3::~ZobControlVector3()
{
	_txt_X->Leave -= _eventX;
	_txt_Y->Leave -= _eventY;
	_txt_Z->Leave -= _eventZ;
	//delete _eventX;
	//delete _eventY;
	//delete _eventZ;
	//delete _txt_X;
	//delete _txt_Y;
	//delete _txt_Z;
	//delete _label;
}

void ZobControlVector3::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	if (this->Focused || this->Disposing || this->IsDisposed)
		return;
	if (_w && _w->ptr)
	{
		ZobVector3* z = (ZobVector3*)_w->ptr;
		if (z)
		{
			if (!_txt_X->Focused)
				_txt_X->Text = String::Format("{0:0.000}", z?z->x:0);
			if (!_txt_Y->Focused)
				_txt_Y->Text = String::Format("{0:0.000}", z ? z->y:0);
			if (!_txt_Z->Focused)
				_txt_Z->Text = String::Format("{0:0.000}", z ? z->z:0);
		}
	}
}

void ZobControlVector3::OnValueChanged(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlVector3^ zb = (ZobControlVector3^)tb->Parent;
	ZobVector3 v;
	std::string sx;
	MarshalString(zb->_txt_X->Text, sx);
	std::string sy;
	MarshalString(zb->_txt_Y->Text, sy);
	std::string sz;
	MarshalString(zb->_txt_Z->Text, sz);
	if (v.FromString(sx, sy, sz))
	{
		ZobVector3* vv = (ZobVector3*)_w->ptr;
		if (_txt_X->Focused)
			vv->x = v.x;
		if (_txt_Y->Focused)
			vv->y = v.y;
		if (_txt_Z->Focused)
			vv->z = v.z;
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

ZobControlFloat::ZobControlFloat(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_txt = gcnew ZobTextBox();
	_txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_txt->AutoSize = false;
	_txt->Width = 140;
	_txt->Height = _height;
	_txt->ReadOnly = w.bReadOnly;
	UpdateControlInternal();
	if (!w.bReadOnly)
	{
		_event = gcnew EventHandler(this, &ZobControlFloat::OnValueChanged);;
		_txt->Leave += _event;
	}
	this->Controls->Add(_label);
	this->Controls->Add(_txt);
	//DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControlFloat::UpdateControl);
}

ZobControlFloat::~ZobControlFloat()
{
	_txt->Leave -= _event;
	//delete _event;
	////delete _label;
}

void ZobControlFloat::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	if (_txt->Focused)
		return;
	if (_w->type == ZobVariablesExposer::eWrapperType_float)
	{
		float* f = (float*)(_w->ptr);
		_txt->Text = String::Format("{0:0.000}", *f?*f:0);
	}
	else
	{
		int* i = (int*)(_w->ptr);
		int ii = (*i);
		//txt->Text = (*i).ToString();
		_txt->Text = String::Format("{0}", ii);
	}	
}

void ZobControlFloat::OnValueChanged(Object^ sender, EventArgs^ e)
{
	TextBox^ t = static_cast<TextBox^>(sender);
	if (_w)
	{
		if (_w->type == ZobVariablesExposer::eWrapperType_int)
		{
			try
			{
				int* f = (int*)_w->ptr;
				int rez = (int)(Convert::ToInt32(t->Text));
				*f = rez;
			}
			catch (...)
			{
				int* f = (int*)_w->ptr;
				t->Text = (*f).ToString();
			}
		}
		else 
		{
			try
			{
				float* f = (float*)_w->ptr;
				t->Text = t->Text->Replace(".", ",");
				float rez = (float)(Convert::ToDouble(t->Text));
				*f = rez;
			}
			catch (...)
			{
				float* f = (float*)_w->ptr;
				t->Text = (*f).ToString();
			}
		}
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

ZobControlBool::ZobControlBool(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	//label->Dock = DockStyle::Fill;
	_label->AutoSize = false;
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_checkBox = gcnew CheckBox();
	_checkBox->AutoSize = false;
	_checkBox->Height = _height;
	_checkBox->Name = TO_MANAGED_STRING(w.internalName.c_str());
	bool* b = (bool*)(w.ptr);
	_checked = *b;
	if (_checked)
		_checkBox->CheckState = CheckState::Checked;
	else
		_checkBox->CheckState = CheckState::Unchecked;
	if (!_w->bReadOnly)
	{
		_event = gcnew EventHandler(this, &ZobControlBool::OnValueChanged);
		_checkBox->Click += _event;
	}
	this->Controls->Add(_label);
	this->Controls->Add(_checkBox);
	DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControlBool::UpdateControl);
}

ZobControlBool::~ZobControlBool()
{
	_checkBox->Click -= _event;
	//delete _label;
	//delete _checkBox;
	//delete _event;
}

void ZobControlBool::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	//if (this->Focused)
	//	return;
	bool* b = (bool*)_w->ptr;
	bool bb = *b;
	if (bb)
		_checkBox->CheckState = CheckState::Checked;
	else
		_checkBox->CheckState = CheckState::Unchecked;
}

void ZobControlBool::OnValueChanged(Object^ sender, EventArgs^ e)
{
	CheckBox^ cb = static_cast<CheckBox^>(sender);
	if (_w)
	{
		_checked = !_checked;
		bool* b = (bool*)_w->ptr;
		*b = _checked?true:false;
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

ZobControlAction::ZobControlAction(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	//label->Dock = DockStyle::Fill;
	_label->AutoSize = false;
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_button = gcnew ZobButton();
	_button->AutoSize = false;
	_button->Height = _height;
	_button->Text = TO_MANAGED_STRING(w.internalName.c_str()); 
	_event = gcnew EventHandler(this, &ZobControlAction::OnValueChanged);
	_button->Click += _event;
	this->Controls->Add(_label);
	this->Controls->Add(_button);
	DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControlAction::UpdateControl);
}

ZobControlAction::~ZobControlAction()
{
	_button->Click -= _event;
	//delete _label;
	//delete _checkBox;
	//delete _event;
}

void ZobControlAction::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
}

void ZobControlAction::OnValueChanged(Object^ sender, EventArgs^ e)
{
	if (_w->callback)
	{
		zobId id = _w->id;
		((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
	}
}


ZobControlEnumCombo::ZobControlEnumCombo(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->AutoSize = false;
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_list = gcnew CheckedListBox();
	_list->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_list->AutoSize = false;
	_list->Width = 140;
	_list->Height = _height;
	List<ZobComboboxItem^>^ items = gcnew List<ZobComboboxItem^>();
	for (int i = 0; i < w.enumNames.size(); i++)
	{
		String^ s = TO_MANAGED_STRING(w.enumNames[i].c_str());
		int idx = w.enumValues[i];
		ZobComboboxItem^ it = gcnew ZobComboboxItem(idx, s, w);
		items->Add(it);
	}
	_list->BindingContext = gcnew System::Windows::Forms::BindingContext();
	_list->DataSource = items;
	int* itemIdx = (int*)(w.ptr);
	_list->SelectedIndex = 0;
	for (int i = 0; i < _list->Items->Count; i++)
	{
		ZobComboboxItem^ it = (ZobComboboxItem^)_list->Items[i];
		if (it->Key == *itemIdx)
		{
			_list->SelectedIndex = i;
			break;
		}
	}
	_event = gcnew EventHandler(this, &ZobControlEnumCombo::OnValueChanged);
	_list->SelectedIndexChanged += _event;
	this->Controls->Add(_label);
	this->Controls->Add(_list);
}

ZobControlEnumCombo::~ZobControlEnumCombo()
{
	_list->SelectedIndexChanged -= _event;
	//delete _event;
	//delete _label;
	//delete _list;
}

void ZobControlEnumCombo::OnValueChanged(Object^ sender, EventArgs^ e)
{

}

void ZobControlEnumCombo::UpdateControlInternal()
{

}

ZobControlEnum::ZobControlEnum(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->AutoSize = false;
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_list = gcnew ComboBox();
	_list->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_list->AutoSize = false;
	_list->Width = 140;
	_list->Height = 3*_height;
	List<ZobComboboxItem^>^ items = gcnew List<ZobComboboxItem^>();
	for (int i = 0; i < w.enumNames.size(); i++)
	{
		String^ s = TO_MANAGED_STRING(w.enumNames[i].c_str());
		int idx = w.enumValues[i];
		ZobComboboxItem^ it = gcnew ZobComboboxItem(idx, s, w);
		items->Add(it);
	}
	_list->BindingContext = gcnew System::Windows::Forms::BindingContext();
	_list->DataSource = items;
	int* itemIdx = (int*)(w.ptr);
	_list->SelectedIndex = 0;
	for (int i = 0; i < _list->Items->Count; i++)
	{
		ZobComboboxItem^ it = (ZobComboboxItem^)_list->Items[i];
		if (it->Key == *itemIdx)
		{
			_list->SelectedIndex = i;
			break;
		}
	}
	_event = gcnew EventHandler(this, &ZobControlEnum::OnValueChanged);
	_list->SelectedIndexChanged += _event;
	this->Controls->Add(_label);
	this->Controls->Add(_list);
}

ZobControlEnum::~ZobControlEnum()
{
	_list->SelectedIndexChanged -= _event;
	//delete _event;
	//delete _label;
	//delete _list;
}

void ZobControlEnum::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	if (this->Focused)
		return;
}

void ZobControlEnum::OnValueChanged(Object^ sender, EventArgs^ e)
{
	ComboBox^ t = static_cast<ComboBox^>(sender);
	if (_w)
	{
		ZobComboboxItem^ it = (ZobComboboxItem^)t->SelectedItem;
		if (it)
		{
			int* i = (int*)_w->ptr;
			*i = it->Key;
		}
		if (_w->callback)
		{
			zobId id = _w->id;
			((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
		}
	}
}

ZobControlZobEntity::ZobControlZobEntity(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 3;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_txt = gcnew ZobTextBox();
	_txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_txt->AutoSize = false;
	_txt->Width = 140;
	_txt->Height = _height;
	_txt->ReadOnly = w.bReadOnly;
	_btn = gcnew ZobButton();
	_btn->AutoSize = false;
	_btn->Width = 40;
	_btn->Height = _height;
	_btn->Text = "+";
	_btn->Click += gcnew EventHandler(this, &ZobControlZobEntity::OnClick);
	ZobObject** pz = (ZobObject**)(w.ptr);
	ZobObject* z = (ZobObject*)(*pz);
	if (z)
	{
		_txt->Text = TO_MANAGED_STRING(z->GetName().c_str());
	}
	else
	{
		_txt->Text = "";
	}
	this->Controls->Add(_label,0 ,0);
	this->Controls->Add(_txt, 1, 0);
	this->Controls->Add(_btn, 2, 0);
}

ZobControlZobEntity::~ZobControlZobEntity()
{
	//delete _txt;
	//delete _label;
}

void ZobControlZobEntity::OnClick(Object^ sender, EventArgs^ e)
{
	if (_w)
	{
		Form^ f = gcnew Form();
		TableLayoutPanel^ tableLayoutPanel1;
		ListView^ listView1;
		TableLayoutPanel^ tableLayoutPanel2;
		Button^ button2;
		Button^ button1;

		listView1 = gcnew System::Windows::Forms::ListView();
		tableLayoutPanel1 = gcnew System::Windows::Forms::TableLayoutPanel();
		tableLayoutPanel2 = gcnew System::Windows::Forms::TableLayoutPanel();
		button1 = gcnew System::Windows::Forms::Button();
		button2 = gcnew System::Windows::Forms::Button();
		tableLayoutPanel1->SuspendLayout();
		tableLayoutPanel2->SuspendLayout();
		SuspendLayout();
		// 
		// tableLayoutPanel1
		// 
		tableLayoutPanel1->ColumnCount = 1;
		tableLayoutPanel1->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50.F));
		tableLayoutPanel1->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50.F));
		tableLayoutPanel1->Controls->Add(listView1, 0, 0);
		tableLayoutPanel1->Controls->Add(tableLayoutPanel2, 0, 1);
		tableLayoutPanel1->Dock = DockStyle::Fill;
		tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
		tableLayoutPanel1->Name = "tableLayoutPanel1";
		tableLayoutPanel1->RowCount = 2;
		tableLayoutPanel1->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 86.94362F));
		tableLayoutPanel1->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 13.05638F));
		tableLayoutPanel1->Size = System::Drawing::Size(382, 337);
		tableLayoutPanel1->TabIndex = 0;
		// 
		// listView1
		// 
		listView1->Dock = DockStyle::Fill;
		listView1->HideSelection = false;
		listView1->Location = System::Drawing::Point(3, 3);
		listView1->Name = "listView1";
		listView1->Size = System::Drawing::Size(376, 286);
		listView1->TabIndex = 0;
		listView1->UseCompatibleStateImageBehavior = false;
		listView1->GridLines = true;
		listView1->AllowColumnReorder = false;
		listView1->FullRowSelect = true;
		listView1->GridLines = true;
		listView1->View = View::Details;
		listView1->Scrollable = true;
		ColumnHeader^ h1 = gcnew ColumnHeader();
		h1->Text = "Name";
		h1->Name = "col1";
		listView1->Columns->Add(h1);
		ColumnHeader^ h2 = gcnew ColumnHeader();
		h2->Text = "Guid";
		h2->Name = "col2";
		listView1->Columns->Add(h2);
		listView1->Columns[0]->Width = 200;
		listView1->Columns[1]->Width = 150;
		listView1->AutoSize = true;
		std::vector<const ZobEntity*> e = ZobEntity::GetEntityByType(_w->strType);
		for (std::vector<const ZobEntity*>::const_iterator it = e.begin(); it != e.end(); it++)
		{
			const ZobEntity* z = *it;
			zobId zid = z->GetIdValue();
			if (ZobEntity::GetType(zid) == ZobEntity::ZobType::type_scene || ZobEntity::GetType(zid) == ZobEntity::ZobType::type_internal)
			{
				System::String^ name = gcnew String(z->GetName().c_str());
				System::String^ guid = gcnew String("");
				guid += zid;
				ListViewItem^ i = gcnew ListViewItem(name);
				i->SubItems->Add(guid);
				listView1->Items->Add(i);
			}
		}
		// 
		// tableLayoutPanel2
		// 
		tableLayoutPanel2->ColumnCount = 2;
		tableLayoutPanel2->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50.F));
		tableLayoutPanel2->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 50.F));
		tableLayoutPanel2->Controls->Add(button2, 1, 0);
		tableLayoutPanel2->Controls->Add(button1, 0, 0);
		tableLayoutPanel2->Dock = DockStyle::Fill;
		tableLayoutPanel2->Location = Point(3, 295);
		tableLayoutPanel2->Name = "tableLayoutPanel2";
		tableLayoutPanel2->RowCount = 1;
		tableLayoutPanel2->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 50.F));
		tableLayoutPanel2->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 50.F));
		tableLayoutPanel2->Size = System::Drawing::Size(376, 39);
		tableLayoutPanel2->TabIndex = 1;
		// 
		// button1
		// 
		button1->Anchor = System::Windows::Forms::AnchorStyles::None;
		button1->ImageAlign = System::Drawing::ContentAlignment::TopRight;
		button1->Location = System::Drawing::Point(56, 8);
		button1->Name = "button1";
		button1->Size = System::Drawing::Size(75, 23);
		button1->TabIndex = 0;
		button1->Text = "button1";
		button1->UseVisualStyleBackColor = true;
		// 
		// button2
		// 
		button2->Anchor = System::Windows::Forms::AnchorStyles::None;
		button2->Location = System::Drawing::Point(244, 8);
		button2->Name = "button2";
		button2->Size = System::Drawing::Size(75, 23);
		button2->TabIndex = 1;
		button2->Text = "button2";
		button2->UseVisualStyleBackColor = true;
		// 
		// ZobEntitySelector
		// 
		f->AutoScaleDimensions = System::Drawing::SizeF(6.F, 13.F);
		f->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		f->ClientSize = System::Drawing::Size(382, 337);
		f->Controls->Add(tableLayoutPanel1);
		f->Name = "ZobEntitySelector";
		f->Text = "ZobEntitySelector";
		tableLayoutPanel1->ResumeLayout(false);
		tableLayoutPanel2->ResumeLayout(false);
		f->ResumeLayout(false);
		f->ShowDialog();
	}
}

void ZobControlZobEntity::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	if (this->Focused)
		return;
}

void ZobControlZobEntity::OnValueChanged(Object^ sender, EventArgs^ e)
{
	if (_w->callback)
	{
		zobId id = _w->id;
		((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
	}
}

ZobGroupBox::ZobGroupBox(String^ name, Image^ im, bool collapsable) :GroupBox()
{
	bToggled = false;
	this->Text = "    "+name;
	this->AutoSize = true;
	this->Dock = DockStyle::Top;
	_image = im;
	if (_image != nullptr)
	{
		_picBox = gcnew PictureBox();
		_picBox->Image = im;
		_picBox->Width = 16;
		_picBox->Height = 16;
		_picBox->Left += 3;
		_event = gcnew MouseEventHandler(this, &ZobGroupBox::OnToggle);
		_event2 = gcnew EventHandler(this, &ZobGroupBox::OnToggle2);
		_picBox->MouseClick += _event;
		_picBox->MouseEnter += _event2;
		this->Controls->Add(_picBox);
	}
}

ZobGroupBox::~ZobGroupBox()
{
	if (_picBox != nullptr)
	{
		_picBox->MouseClick -= _event;
	}
	for (int i = 0; i < this->Controls->Count; i++)
	{
		//delete(this->Controls[i]);
	}
	//delete _event;
	//delete _label;
}

void ZobGroupBox::OnToggle2(Object^ sender, EventArgs^ e)
{
}

void ZobGroupBox::OnToggle(Object^ sender, MouseEventArgs^ e)
{
	PictureBox^ b = (PictureBox^)sender;
	ZobGroupBox^ z = (ZobGroupBox^)b->Parent;
	if (bToggled)
	{
		bToggled = false;
		z->AutoSize = true;
		z->Refresh();
	}
	else
	{
		bToggled = true;
		z->AutoSize = false;
		z->Height = 25;
	}
}

ZobControlTreeNode::ZobControlTreeNode(String^ zobObjectGuid) :TreeNode()
{
	std::string s;
	m_zobObjectGuid = zobObjectGuid;
	MarshalString(zobObjectGuid, s);
	zobId zid = ZobEntity::ZobIdFromString(s);
	this->ToolTipText = m_zobObjectGuid;
	ZobEntityInfo^ zi = gcnew ZobEntityInfo(zid);
	m_isSelectable = zi->_isSelectable;
	m_isReadOnly = zi->_isReadOnly;
	this->ImageIndex = (int)zi->_imgType;
	this->SelectedImageIndex = this->ImageIndex;
	this->ForeColor = zi->_color;
}

ZobControlTreeNode::~ZobControlTreeNode()
{

}

void ZobControlTreeNode::OnClick(Object^ sender, TreeNodeMouseClickEventArgs^ e)
{
	ZobControlTreeview^ tv = (ZobControlTreeview^)sender;
	int x = e->Location.X - tv->Bounds.Left;
	if (x > tv->Bounds.Width - 40 && x < tv->Bounds.Width - 20)
	{
		m_locked = !m_locked;
	}
	else if (x > tv->Bounds.Width - 40 && x < tv->Bounds.Width)
	{
		m_visible = !m_visible;
	}
	tv->Invalidate();
}

ZobControlTreeNode^ ZobControlTreeNode::GetChildNode(String^ guid) 
{
	for (int i = 0; i < this->Nodes->Count; i++)
	{
		ZobControlTreeNode^ n = (ZobControlTreeNode^)Nodes[i];
		if (n->m_zobObjectGuid == guid)
		{
			return n;
		}
	}
	return nullptr;
}

ZobControlTreeview::ZobControlTreeview():TreeView() 
{
	this->DrawMode = TreeViewDrawMode::OwnerDrawAll;
}


void ZobControlTreeview::OnDrawNode(DrawTreeNodeEventArgs^ e)
{
	//e->Bounds.Left += 40;
	e->DrawDefault = true;
	//TreeView::OnDrawNode(e);
	int x;
	VisualStyles::CheckBoxState s;
	x = this->Bounds.Width - 40;
	Drawing::Rectangle r = Drawing::Rectangle(x-1, e->Bounds.Top-1, 40, e->Bounds.Height);
	e->Graphics->FillRectangle(SystemBrushes::ActiveBorder, r);
	ZobControlTreeNode^ n = (ZobControlTreeNode^)e->Node;
	x = this->Bounds.Width - 20;
	Point p = Point(x, e->Bounds.Top);
	s = n->m_visible?VisualStyles::CheckBoxState::CheckedNormal: VisualStyles::CheckBoxState::UncheckedNormal;
	CheckBoxRenderer::DrawCheckBox(e->Graphics, p, s);
	x = this->Bounds.Width - 40;
	p = Point(x, e->Bounds.Top);
	s = n->m_locked ? VisualStyles::CheckBoxState::CheckedNormal : VisualStyles::CheckBoxState::UncheckedNormal;
	CheckBoxRenderer::DrawCheckBox(e->Graphics, p, s);
	TreeView::OnDrawNode(e);
}

void ZobControlTreeview::UpdateZobControl()
{
}

ZobPropertiesLayout::ZobPropertiesLayout(String^ name):TableLayoutPanel()
{
	this->AutoSize = true;
	this->ColumnCount = 1;
	_label = gcnew ZobLabel();
	_label->AutoSize = true;
	_label->Text = name;
	_label->TextAlign = Drawing::ContentAlignment::TopCenter;
	this->Controls->Add(_label);
	//label->Font->Bold = true;
}

ZobPropertiesLayout::~ZobPropertiesLayout()
{
	//delete _label;
}

ZobPropertiesContainer::ZobPropertiesContainer() : TableLayoutPanel()
{
	this->Dock = DockStyle::Fill;
	this->ColumnCount = 1;
	this->AutoSize = true;
	//this->BackColor = Drawing::Color::Beige;
}

ZobControlZobId::ZobControlZobId(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	_label = gcnew ZobLabel();
	_label->Text = TO_MANAGED_STRING(w.name.c_str());
	_label->Width = _labelWidth;
	_label->Height = _height;
	_label->TextAlign = _alignment;
	_txt = gcnew ZobLabel();
	_txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_txt->AutoSize = false;
	_txt->Width = 140;
	_txt->Height = _height;
	_txt->TextAlign = _alignment;
	//txt->ReadOnly = true;
	unsigned long long* i = (unsigned long long*)(w.ptr);
	_txt->Text = (*i).ToString();
	this->Controls->Add(_label);
	this->Controls->Add(_txt);
}

ZobControlZobId::~ZobControlZobId()
{
	//delete _label;
	//delete _txt;
}

ZobControlColor::ZobControlColor(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	String^ internalName = TO_MANAGED_STRING(w.internalName.c_str());
	String^ name = TO_MANAGED_STRING(w.name.c_str());
	ZobColor* v = (ZobColor*)(w.ptr);
	this->AutoSize = true;
	this->ColumnCount = 6;
	this->RowCount = 1;
	this->Name = name;
	int width = 80;
	int txtW = 30;
	int btnW = 40;
	_label = gcnew ZobLabel();
	_label->Text = name;
	_label->Width = width;
	_label->Height = _height;
	_label->AutoSize = false;
	_label->TextAlign = _alignment;
	_label->Width = _labelWidth;

	_txt_A = gcnew ZobTextBox();
	_txt_A->AutoSize = false;
	_txt_A->Name = name;
	_txt_A->Width = txtW;
	_txt_A->Height = _height;
	_txt_A->ReadOnly = _w->bReadOnly;

	_txt_R = gcnew ZobTextBox();
	_txt_R->AutoSize = false;
	_txt_R->Name = name;
	_txt_R->Width = txtW;
	_txt_R->Height = _height;
	_txt_R->ReadOnly = _w->bReadOnly;

	_txt_G = gcnew ZobTextBox();
	_txt_G->AutoSize = false;
	_txt_G->Name = name;
	_txt_G->Width = txtW;
	_txt_G->Height = _height;
	_txt_G->ReadOnly = _w->bReadOnly;

	_txt_B = gcnew ZobTextBox();
	_txt_B->Name = name;
	_txt_B->Width = txtW;
	_txt_B->AutoSize = false;
	_txt_B->Height = _height;
	_txt_B->AutoSize = false;
	_txt_B->ReadOnly = _w->bReadOnly;

	_btn = gcnew ZobButton();
	_btn->AutoSize = false;
	_btn->Width = btnW;
	_btn->Height = _height;
	_btn->Text = "+";
	

	UpdateControlInternal();

	_eventA += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventR += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventG += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventB += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_txt_A->Leave += _eventA;
	_txt_R->Leave += _eventR;
	_txt_G->Leave += _eventG;
	_txt_B->Leave += _eventB;
	_btn->Click += gcnew EventHandler(this, &ZobControlColor::OnClickColor);

	this->Controls->Add(_label, 0, 0);
	this->Controls->Add(_txt_A, 1, 0);
	this->Controls->Add(_txt_R, 2, 0);
	this->Controls->Add(_txt_G, 3, 0);
	this->Controls->Add(_txt_B, 4, 0);
	this->Controls->Add(_btn, 5, 0);
}

ZobControlColor::~ZobControlColor()
{
	_txt_A->Leave -= _eventA;
	_txt_R->Leave -= _eventR;
	_txt_G->Leave -= _eventG;
	_txt_B->Leave -= _eventB;
	//delete _label;
	//delete _btn;
	//delete _eventA;
	//delete _eventR;
	//delete _eventG;
	//delete _eventB;
	//delete _txt_A;
	//delete _txt_R;
	//delete _txt_G;
	//delete _txt_B;
}

void ZobControlColor::OnClickColor(Object^ sender, EventArgs^ e)
{
	ZobColor* vv = (ZobColor*)_w->ptr;
	if (vv)
	{
		Button^ tb = (Button^)sender;
		ColorDialog^ d = gcnew ColorDialog();
		//System::Drawing::Color c = System::Drawing::Color::FromArgb(a, r, g, b);
		d->Color = tb->BackColor;
		if (d->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			System::Drawing::Color c = d->Color;
			int a = (int)(c.A);
			int r = (int)(c.R);
			int g = (int)(c.G);
			int b = (int)(c.B);
			vv->Set(a, r, g, b);
			//delete d;
		}
	}
}

void ZobControlColor::UpdateControlInternal()
{
	if (!IsControlOk())
	{
		return;
	}
	if (this->Focused || this->Disposing || this->IsDisposed)
		return;
	if (_w && _w->ptr)
	{
		ZobColor* z = (ZobColor*)_w->ptr;
		int a = z?z->GetAlpha():0;
		int r = z?z->GetRed():0;
		int g = z?z->GetGreen():0;
		int b = z?z->GetBlue():0;
		if (z)
		{
			if (!_txt_A->Focused)
				_txt_A->Text = String::Format("{0:0}", a);
			if (!_txt_R->Focused)
				_txt_R->Text = String::Format("{0:0}", r);
			if (!_txt_G->Focused)
				_txt_G->Text = String::Format("{0:0}", g);
			if (!_txt_B->Focused)
				_txt_B->Text = String::Format("{0:0}", b);
			_btn->BackColor = System::Drawing::Color::FromArgb(a, r, g, b);
		}
	}
}

void ZobControlColor::OnValueChanged(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlColor^ zb = (ZobControlColor^)tb->Parent;
	ZobColor v;
	ZobColor* vv = (ZobColor*)_w->ptr;
	if (vv)
	{
		int alpha = vv->GetAlpha();
		int red = vv->GetRed();
		int green = vv->GetGreen();
		int blue = vv->GetBlue();
		if (zb->_txt_A->Focused)
			Int32::TryParse(zb->_txt_A->Text, alpha);
		if(zb->_txt_R->Focused)
			Int32::TryParse(zb->_txt_R->Text, red);
		if (zb->_txt_G->Focused)
			Int32::TryParse(zb->_txt_G->Text, green);
		if (zb->_txt_B->Focused)
			Int32::TryParse(zb->_txt_B->Text, blue);
		if (true)
		{
			vv->Set(alpha, red, green, blue);
			if (_w->callback)
			{
				zobId id = _w->id;
				((ZobVariablesExposer::wrapperCallback)_w->callback)(id);
			}
		}
	}
}