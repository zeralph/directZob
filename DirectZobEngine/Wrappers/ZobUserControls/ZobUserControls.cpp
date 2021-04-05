#include "ZobUserControls.h"

using namespace CLI;

ZobControlString::ZobControlString(String^ name, String^ text, String^ value, bool readOnly, EventArgs^ OnChanged):TableLayoutPanel()
{
	this->AutoSize = true;
	this->ColumnCount = 2;
	this->RowCount = 1;
	Label^ label = gcnew Label();
	label->Text = text;
	label->Width = 140;
	label->Height = 20;
	label->TextAlign = ContentAlignment::BottomRight;
	TextBox^ txt = gcnew TextBox();
	txt->Name = name;
	txt->Width = 140;
	txt->Height = 20;
	txt->ReadOnly = readOnly;
	txt->Text = value;
	if (!readOnly)
	{
		//txt->Leave += gcnew EventHandler(this, OnChanged);
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
	int y = 0;
	y++;
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
	labelx->Height = 20;
	labelx->TextAlign = ContentAlignment::BottomRight;	
	txt_X = gcnew TextBox();
	txt_X->Name = name;
	txt_X->Width = 140;
	txt_X->Height = 20;
	txt_X->ReadOnly = readOnly;

	Label^ labely = gcnew Label();
	labely->Text = text + " | Y";
	labely->Width = 140;
	labely->Height = 20;
	labely->TextAlign = ContentAlignment::BottomRight;
	txt_Y = gcnew TextBox();
	txt_Y->Name = name;
	txt_Y->Width = 140;
	txt_Y->Height = 20;
	txt_Y->ReadOnly = readOnly;

	//txt_Y->Text = v->y.ToString();

	Label^ labelz = gcnew Label();
	labelz->Text = "| Z";
	labelz->Width = 140;
	labelz->Height = 20;
	labelz->TextAlign = ContentAlignment::BottomRight;
	txt_Z = gcnew TextBox();
	txt_Z->Name = name;
	txt_Z->Width = 140;
	txt_Z->Height = 20;
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
		txt_X->Text = String::Format("{0:0.000}",m_zobVector3->x);
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