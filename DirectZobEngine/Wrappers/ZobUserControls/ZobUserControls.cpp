#include "ZobUserControls.h"
#include "../DirectZobWrapper.h"
#define TO_MANAGED_STRING(x) gcnew String(x);

using namespace CLI;

static int _height = 15;
static void MarshalString(System::String^ s, std::string& os) {
	using namespace System::Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}

ZobControlString::ZobControlString(ZobVariablesExposer::wrapperData* w):TableLayoutPanel()
{
	this->Name = TO_MANAGED_STRING(w->internalName.c_str());
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = TO_MANAGED_STRING(w->name.c_str());
	label->Width = 140;
	label->Height = _height;
	label->AutoSize = false;
	label->TextAlign = ContentAlignment::BottomRight;
	txt = gcnew TextBox();
	txt->AutoSize = false;
	txt->Width = 140;
	txt->Height = _height;
	txt->ReadOnly = (bool)w->bReadOnly;
	std::string* s = (std::string*)(w->ptr);
	txt->Text = TO_MANAGED_STRING(s->c_str());
	if (!txt->ReadOnly)
	{
		txt->Leave += gcnew EventHandler(this, &ZobControlString::OnValueChangedInternal);
	}
	this->Controls->Add(label);
	this->Controls->Add(txt);
}

ZobGroupBox::ZobGroupBox(String^ name, bool collapsable) :GroupBox()
{
	bToggled = false;
	this->Text = name;
	this->AutoSize = true;
	Label^ b = gcnew Label();
	b->Text = "-";
	b->Width = 10;
	b->Height = 10;
	b->Top = this->Top;
	b->Left = this->Left;
	b->BackColor = Drawing::Color::WhiteSmoke;
	b->BorderStyle = BorderStyle::FixedSingle;
	this->Controls->Add(b);
	b->Click += gcnew EventHandler(this, &ZobGroupBox::OnToggle);
}

ZobControlTreeNode::ZobControlTreeNode(String^ zobObjectGuid) :TreeNode()
{
	m_zobObjectGuid = zobObjectGuid;
	this->ToolTipText = m_zobObjectGuid;
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

ZobControlVector3::ZobControlVector3(String^ name, String^ text, ZobVector3* v, bool readOnly, EventArgs^ OnChanged) :TableLayoutPanel()
{
	m_zobVector3 = v;
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 3;
	this->Name = name;
	Label^ labelx = gcnew Label();
	labelx->Text = "| X";
	labelx->Width = 140;
	labelx->Height = _height;
	labelx->AutoSize = false;
	labelx->TextAlign = ContentAlignment::BottomRight;	
	txt_X = gcnew TextBox();
	txt_X->AutoSize = false;
	txt_X->Name = name;
	txt_X->Width = 140;
	txt_X->Height = _height;
	txt_X->ReadOnly = readOnly;

	Label^ labely = gcnew Label();
	labely->Text = text + " | Y";
	labely->Width = 140;
	labely->Height = _height;
	labely->AutoSize = false;
	labely->TextAlign = ContentAlignment::BottomRight;
	txt_Y = gcnew TextBox();
	txt_Y->AutoSize = false;
	txt_Y->Name = name;
	txt_Y->Width = 140;
	txt_Y->Height = _height;
	txt_Y->ReadOnly = readOnly;

	//txt_Y->Text = v->y.ToString();

	Label^ labelz = gcnew Label();
	labelz->Text = "| Z";
	labelz->Width = 140;
	labelz->Height = _height;
	labelz->AutoSize = false;
	labelz->TextAlign = ContentAlignment::BottomRight;
	txt_Z = gcnew TextBox();
	txt_Z->Name = name;
	txt_Z->Width = 140;
	txt_Z->AutoSize = false;
	txt_Z->Height = _height;
	txt_X->AutoSize = false;
	txt_Z->ReadOnly = readOnly;

	UpdateZobControlInternal();

	txt_X->Leave += gcnew EventHandler(this, &ZobControlVector3::OnValueChangedInternal);
	txt_Y->Leave += gcnew EventHandler(this, &ZobControlVector3::OnValueChangedInternal);
	txt_Z->Leave += gcnew EventHandler(this, &ZobControlVector3::OnValueChangedInternal);

	this->Controls->Add(labelx);
	this->Controls->Add(txt_X);
	this->Controls->Add(labely);
	this->Controls->Add(txt_Y);
	this->Controls->Add(labelz);
	this->Controls->Add(txt_Z);
}

void ZobControlVector3::UpdateZobControl()
{
	this->Invoke(gcnew Action(this, &CLI::ZobControlVector3::UpdateZobControlInternal));
}

void ZobControlVector3::UpdateZobControlInternal()
{
	if(!txt_X->Focused)
		txt_X->Text = String::Format("{0:0.000}", m_zobVector3->x);
	if (!txt_Y->Focused)
		txt_Y->Text = String::Format("{0:0.000}", m_zobVector3->y);
	if (!txt_Z->Focused)
		txt_Z->Text = String::Format("{0:0.000}", m_zobVector3->z);
}

void ZobControlVector3::OnValueChangedInternal(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlVector3^ zb = (ZobControlVector3^ )tb->Parent;
	OnChangeEvent(zb, e);
}

void ZobControlString::OnValueChangedInternal(Object^ sender, EventArgs^ e)
{
	TextBox^ tb = (TextBox^)sender;
	ZobControlString^ zb = (ZobControlString^)tb->Parent;
	OnChangeEvent(zb, e);
}

