#include "ZobUserControls.h"
#include "../DirectZobWrapper.h"
#define TO_MANAGED_STRING(x) gcnew String(x);

using namespace CLI;
static ContentAlignment _alignment = ContentAlignment::MiddleLeft;
static int _height = 15;
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
}

ZobControl::~ZobControl()
{
	_w = NULL;
	DirectZobWrapperEvents::OnEditorUpdateEvent -= _updateEvent;
	delete _updateEvent;
	_updateEvent = nullptr;
}

void ZobControl::UpdateControl() 
{ 
	if (this->IsHandleCreated && !this->IsDisposed && !this->Disposing)
	{
		this->Invoke(gcnew Action(this, &ZobControl::UpdateControlInternal));
	} 
}

ZobControlString::ZobControlString(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->Name = TO_MANAGED_STRING(w.internalName.c_str());
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	label->Width = 100;
	label->Height = _height;
	label->AutoSize = false;
	label->TextAlign = _alignment;
	txt = gcnew TextBox();
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->ReadOnly = (bool)w.bReadOnly;
	std::string* s = (std::string*)(w.ptr);
	txt->Text = TO_MANAGED_STRING(s->c_str());
	if (!txt->ReadOnly)
	{
		_event = gcnew EventHandler(this, &ZobControlString::OnValueChanged);;
		txt->Leave += _event;
	}
	this->Controls->Add(label, 0, 0);
	this->Controls->Add(txt, 1, 0);
}
ZobControlString::~ZobControlString()
{
	txt->Leave -= _event;
	delete _event;
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
	}
}

ZobControlFilePath::ZobControlFilePath(const ZobVariablesExposer::wrapperData& w):ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 3;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	//label->Dock = DockStyle::Fill;
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
	txt = gcnew TextBox();
	txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->ReadOnly = w.bReadOnly;
	btn = gcnew Button();
	btn->AutoSize = false;
	btn->Width = 20;
	btn->Height = _height;
	btn->Text = "...";
	//txt->MaximumSize = Drawing::Size(100, 20);
	std::string s;
	ZobFilePath* zp = (ZobFilePath*)(w.ptr);
	txt->Text = TO_MANAGED_STRING(zp->name.c_str());
	txt->ReadOnly = true;
	if (!w.bReadOnly)
	{
		//txt->Leave += gcnew EventHandler(this, &ZobControlFilePath::OnValueChanged);
		_event = gcnew EventHandler(this, &ZobControlFilePath::OnOpen);
		btn->Click += _event;
	}
	this->Controls->Add(label);
	this->Controls->Add(txt);
	this->Controls->Add(btn);
}

ZobControlFilePath::~ZobControlFilePath()
{
	btn->Click -= _event;
	delete _event;
}

void ZobControlFilePath::OnOpen(Object^ sender, EventArgs^ e)
{
	OpenFileDialog^ f = gcnew OpenFileDialog();
	f->Filter = "OBJ files (*.obj)|*.obj|FBX files (*.fbx)|*.fbx";
	f->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &ZobControlFilePath::OnOk);
	f->ShowDialog();
}

void ZobControlFilePath::OnOk(Object^ sender, System::ComponentModel::CancelEventArgs^ e)
{
	OpenFileDialog^ f = (OpenFileDialog^)sender;
	String^ fullPath = f->FileName;
	System::IO::FileInfo^ fi = gcnew System::IO::FileInfo(fullPath);
	String^ dir = fi->DirectoryName;
	String^ file = fi->Name;
	txt->Text = file;
	if (_w)
	{
		std::string s;
		MarshalString(file+";"+dir+";"+file, s);
		ZobFilePath* zp = (ZobFilePath*)_w->ptr;
		zp->Unserialize(s);
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
	Label^ label = gcnew Label();
	label->Text = name;
	label->Width = width;
	label->Height = _height;
	label->AutoSize = false;
	label->TextAlign = _alignment;
	label->Width = 100;
	txt_X = gcnew TextBox();
	txt_X->AutoSize = false;
	txt_X->Name = name;
	txt_X->Width = width;
	txt_X->Height = _height;
	txt_X->ReadOnly = _w->bReadOnly;

	txt_Y = gcnew TextBox();
	txt_Y->AutoSize = false;
	txt_Y->Name = name;
	txt_Y->Width = width;
	txt_Y->Height = _height;
	txt_Y->ReadOnly = _w->bReadOnly;

	txt_Z = gcnew TextBox();
	txt_Z->Name = name;
	txt_Z->Width = width;
	txt_Z->AutoSize = false;
	txt_Z->Height = _height;
	txt_X->AutoSize = false;
	txt_Z->ReadOnly = _w->bReadOnly;

	UpdateControlInternal();
	_eventX = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	_eventY = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	_eventZ = gcnew EventHandler(this, &ZobControlVector3::OnValueChanged);
	txt_X->Leave += _eventX;
	txt_Y->Leave += _eventY;
	txt_Z->Leave += _eventZ;

	this->Controls->Add(label, 0, 0);
	this->Controls->Add(txt_X, 1, 0);
	this->Controls->Add(txt_Y, 2, 0);
	this->Controls->Add(txt_Z, 3, 0);

}

ZobControlVector3::~ZobControlVector3()
{
	txt_X->Leave -= _eventX;
	txt_Y->Leave -= _eventY;
	txt_Z->Leave -= _eventZ;
	delete _eventX;
	delete _eventY;
	delete _eventZ;
}

void ZobControlVector3::UpdateControlInternal()
{
	if (this->Focused || this->Disposing || this->IsDisposed)
		return;
	if (_w && _w->ptr)
	{
		ZobVector3* z = (ZobVector3*)_w->ptr;
		if (z)
		{
			if (!txt_X->Focused)
				txt_X->Text = String::Format("{0:0.000}", z->x);
			if (!txt_Y->Focused)
				txt_Y->Text = String::Format("{0:0.000}", z->y);
			if (!txt_Z->Focused)
				txt_Z->Text = String::Format("{0:0.000}", z->z);
		}
	}
}

void ZobControlVector3::OnValueChanged(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlVector3^ zb = (ZobControlVector3^)tb->Parent;
	ZobVector3 v;
	std::string sx;
	MarshalString(zb->txt_X->Text, sx);
	std::string sy;
	MarshalString(zb->txt_Y->Text, sy);
	std::string sz;
	MarshalString(zb->txt_Z->Text, sz);
	if (v.FromString(sx, sy, sz))
	{
		ZobVector3* vv = (ZobVector3*)_w->ptr;
		if (txt_X->Focused)
			vv->x = v.x;
		if (txt_Y->Focused)
			vv->y = v.y;
		if (txt_Z->Focused)
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
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
	txt = gcnew TextBox();
	txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->ReadOnly = w.bReadOnly;
	UpdateControlInternal();
	if (!w.bReadOnly)
	{
		_event = gcnew EventHandler(this, &ZobControlFloat::OnValueChanged);;
		txt->Leave += _event;
	}
	this->Controls->Add(label);
	this->Controls->Add(txt);
	//DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControlFloat::UpdateControl);
}

ZobControlFloat::~ZobControlFloat()
{
	txt->Leave -= _event;
	delete _event;
}

void ZobControlFloat::UpdateControlInternal()
{
	if (txt->Focused)
		return;
	if (_w->type == ZobVariablesExposer::eWrapperType_float)
	{
		float* f = (float*)(_w->ptr);
		txt->Text = String::Format("{0:0.000}", *f);
	}
	else
	{
		int* i = (int*)(_w->ptr);
		int ii = (*i);
		//txt->Text = (*i).ToString();
		txt->Text = String::Format("{0}", ii);
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
	}
}

ZobControlBool::ZobControlBool(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	//label->Dock = DockStyle::Fill;
	label->AutoSize = false;
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
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
	this->Controls->Add(label);
	this->Controls->Add(_checkBox);
	DirectZobWrapperEvents::OnEditorUpdateEvent += gcnew DirectZobWrapperEvents::OnEditorUpdate(this, &ZobControlBool::UpdateControl);
}

ZobControlBool::~ZobControlBool()
{
	_checkBox->Click -= _event;
	delete _event;
}

void ZobControlBool::UpdateControlInternal()
{
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
	}
}

ZobControlEnum::ZobControlEnum(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	label->AutoSize = false;
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
	_list = gcnew ComboBox();
	_list->Name = TO_MANAGED_STRING(w.internalName.c_str());
	_list->AutoSize = false;
	_list->Width = 140;
	_list->Height = _height;
	List<ComboboxItem^>^ items = gcnew List<ComboboxItem^>();
	for (int i = 0; i < w.enumNames.size(); i++)
	{
		String^ s = TO_MANAGED_STRING(w.enumNames[i].c_str());
		int idx = w.enumValues[i];
		ComboboxItem^ it = gcnew ComboboxItem(idx, s, w);
		items->Add(it);
	}
	_list->BindingContext = gcnew System::Windows::Forms::BindingContext();
	_list->DataSource = items;
	int* itemIdx = (int*)(w.ptr);
	_list->SelectedIndex = 0;
	for (int i = 0; i < _list->Items->Count; i++)
	{
		ComboboxItem^ it = (ComboboxItem^)_list->Items[i];
		if (it->Key == *itemIdx)
		{
			_list->SelectedIndex = i;
			break;
		}
	}
	_event = gcnew EventHandler(this, &ZobControlEnum::OnValueChanged);
	_list->SelectedIndexChanged += _event;
	this->Controls->Add(label);
	this->Controls->Add(_list);
}

ZobControlEnum::~ZobControlEnum()
{
	_list->SelectedIndexChanged -= _event;
	delete _event;
	delete _list;
}

void ZobControlEnum::UpdateControlInternal()
{
	if (this->Focused)
		return;
}

void ZobControlEnum::OnValueChanged(Object^ sender, EventArgs^ e)
{
	ComboBox^ t = static_cast<ComboBox^>(sender);
	if (_w)
	{
		ComboboxItem^ it = (ComboboxItem^)t->SelectedItem;
		if (it)
		{
			int* i = (int*)_w->ptr;
			*i = it->Key;
		}
	}
}

ZobControlZobObject::ZobControlZobObject(const ZobVariablesExposer::wrapperData& w) :ZobControl(w)
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
	txt = gcnew TextBox();
	txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->ReadOnly = w.bReadOnly;
	ZobObject** pz = (ZobObject**)(w.ptr);
	ZobObject* z = (ZobObject*)(*pz);
	if (z)
	{
		txt->Text = TO_MANAGED_STRING(z->GetName().c_str());
	}
	else
	{
		txt->Text = "";
	}
	this->Controls->Add(label,0 ,0);
	this->Controls->Add(txt, 1, 0);
}

void ZobControlZobObject::UpdateControlInternal()
{
	if (this->Focused)
		return;
}

void ZobControlZobObject::OnValueChanged(Object^ sender, EventArgs^ e)
{
}

ZobGroupBox::ZobGroupBox(String^ name, bool collapsable) :GroupBox()
{
	bToggled = false;
	this->Text = name;
	this->AutoSize = true;
	this->Dock = DockStyle::Fill;
	_label = gcnew Label();
	_label->Text = "-";
	_label->Width = 10;
	_label->Height = 10;
	_label->Top = this->Top;
	_label->Left = this->Left;
	_label->BackColor = Drawing::Color::WhiteSmoke;
	_label->BorderStyle = BorderStyle::FixedSingle;
	this->Controls->Add(_label);
	_event = gcnew EventHandler(this, &ZobGroupBox::OnToggle);
	_label->Click += _event;
}

ZobGroupBox::~ZobGroupBox()
{
	_label->Click -= _event;
	delete _event;
	delete _label;
}

ZobControlTreeNode::ZobControlTreeNode(String^ zobObjectGuid, bool isEditable) :TreeNode()
{
	std::string s;
	m_zobObjectGuid = zobObjectGuid;
	MarshalString(zobObjectGuid, s);
	ZOBGUID z = ZOBGUID(s);
	this->ToolTipText = m_zobObjectGuid;
	m_isEditable = isEditable;
	if (z.GetType() != ZOBGUID::type_scene && z.GetType() != ZOBGUID::type_internal)
	{
		this->ForeColor = Color::Red;
	}
	else if (z.GetSubType() == ZOBGUID::subtype_zobCamera)
	{
		this->ForeColor = Color::Brown;
	}
	else if (z.GetSubType() == ZOBGUID::subtype_zobLight)
	{
		this->ForeColor = Color::Green;
	}
	else if (z.GetSubType() == ZOBGUID::subtype_zobOject)
	{
		this->ForeColor = Color::Blue;
	}
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

void ZobGroupBox::OnToggle(Object^ sender, EventArgs^ e)
{
	Label^ b = (Label^)sender;
	ZobGroupBox^ z = (ZobGroupBox^)b->Parent;
	if (bToggled)
	{
		bToggled = false;
		z->AutoSize = true;
		b->Text = "-";
		z->Refresh();
	}
	else
	{
		bToggled = true;
		b->Text = "+";
		z->AutoSize = false;
		z->Height = 25;
	}
}

ZobControlTreeview::ZobControlTreeview():TreeView() 
{
}

void ZobControlTreeview::UpdateZobControl()
{
}

ZobPropertiesLayout::ZobPropertiesLayout(String^ name):TableLayoutPanel()
{
	this->AutoSize = true;
	this->ColumnCount = 1;
	Label^ label = gcnew Label();
	label->AutoSize = true;
	label->Text = name;
	label->TextAlign = Drawing::ContentAlignment::TopCenter;
	this->Controls->Add(label);
	//label->Font->Bold = true;
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
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w.name.c_str());
	label->Width = 100;
	label->Height = _height;
	label->TextAlign = _alignment;
	txt = gcnew Label();
	txt->Name = TO_MANAGED_STRING(w.internalName.c_str());
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->TextAlign = _alignment;
	//txt->ReadOnly = true;
	unsigned long long* i = (unsigned long long*)(w.ptr);
	txt->Text = (*i).ToString();
	this->Controls->Add(label);
	this->Controls->Add(txt);
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
	Label^ label = gcnew Label();
	label->Text = name;
	label->Width = width;
	label->Height = _height;
	label->AutoSize = false;
	label->TextAlign = _alignment;
	label->Width = 100;

	txt_A = gcnew TextBox();
	txt_A->AutoSize = false;
	txt_A->Name = name;
	txt_A->Width = 40;
	txt_A->Height = _height;
	txt_A->ReadOnly = _w->bReadOnly;

	txt_R = gcnew TextBox();
	txt_R->AutoSize = false;
	txt_R->Name = name;
	txt_R->Width = 40;
	txt_R->Height = _height;
	txt_R->ReadOnly = _w->bReadOnly;

	txt_G = gcnew TextBox();
	txt_G->AutoSize = false;
	txt_G->Name = name;
	txt_G->Width = 40;
	txt_G->Height = _height;
	txt_G->ReadOnly = _w->bReadOnly;

	txt_B = gcnew TextBox();
	txt_B->Name = name;
	txt_B->Width = 40;
	txt_B->AutoSize = false;
	txt_B->Height = _height;
	txt_B->AutoSize = false;
	txt_B->ReadOnly = _w->bReadOnly;

	btn = gcnew Button();
	btn->AutoSize = false;
	btn->Width = 40;
	btn->Height = _height;
	btn->Text = "...";
	

	UpdateControlInternal();

	_eventA += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventR += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventG += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	_eventB += gcnew EventHandler(this, &ZobControlColor::OnValueChanged);
	txt_A->Leave += _eventA;
	txt_R->Leave += _eventR;
	txt_G->Leave += _eventG;
	txt_B->Leave += _eventB;
	btn->Click += gcnew EventHandler(this, &ZobControlColor::OnClickColor);

	this->Controls->Add(label, 0, 0);
	this->Controls->Add(txt_A, 1, 0);
	this->Controls->Add(txt_R, 2, 0);
	this->Controls->Add(txt_G, 3, 0);
	this->Controls->Add(txt_B, 4, 0);
	this->Controls->Add(btn, 5, 0);
}

ZobControlColor::~ZobControlColor()
{
	txt_A->Leave -= _eventA;
	txt_R->Leave -= _eventR;
	txt_G->Leave -= _eventG;
	txt_B->Leave -= _eventB;
	delete _eventA;
	delete _eventR;
	delete _eventG;
	delete _eventB;
	delete txt_A;
	delete txt_R;
	delete txt_G;
	delete txt_B;
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
			delete d;
		}
	}
}

void ZobControlColor::UpdateControlInternal()
{
	if (this->Focused || this->Disposing || this->IsDisposed)
		return;
	if (_w && _w->ptr)
	{
		ZobColor* z = (ZobColor*)_w->ptr;
		int a = z->GetAlpha();
		int r = z->GetRed();
		int g = z->GetGreen();
		int b = z->GetBlue();
		if (z)
		{
			if (!txt_A->Focused)
				txt_A->Text = String::Format("{0:0}", a);
			if (!txt_R->Focused)
				txt_R->Text = String::Format("{0:0}", r);
			if (!txt_G->Focused)
				txt_G->Text = String::Format("{0:0}", g);
			if (!txt_B->Focused)
				txt_B->Text = String::Format("{0:0}", b);
			btn->BackColor = System::Drawing::Color::FromArgb(a, r, g, b);
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
		if (zb->txt_A->Focused)
			Int32::TryParse(zb->txt_A->Text, alpha);
		if(zb->txt_R->Focused)
			Int32::TryParse(zb->txt_R->Text, red);
		if (zb->txt_G->Focused)
			Int32::TryParse(zb->txt_G->Text, green);
		if (zb->txt_B->Focused)
			Int32::TryParse(zb->txt_B->Text, blue);
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