#ifdef _WINDLL
#include "ZobObjectWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobLightWrapper.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/Camera.h"
#include "../DirectZob.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject):ManagedObject(zobObject, false)
	{
		m_id = zobObject->GetIdValue();
	}

	ZobObjectWrapper::~ZobObjectWrapper()
	{
		m_Instance = NULL;
	}

	UserControl^ ZobObjectWrapper::FillBehaviorsControl()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			const std::vector<ZobBehavior*>* behaviors = z->GetBehaviors();
			if (behaviors->size() > 0)
			{
				UserControl^ c = gcnew UserControl();
				c->Width = 300;
				c->Height = 0;
				for (int i = 0; i < behaviors->size(); i++)
				{
					ZobBehavior* zb = behaviors->at(i);
					GroupBox^ gb = FillBehaviorControl(zb);
					c->Controls->Add(gb);
					gb->Top = c->Height;
					c->Height = c->Height + gb->Height + 2;
				}
				return c;
			}
		}
		return nullptr;
	}

	GroupBox^ ZobObjectWrapper::FillBehaviorControl(ZobBehavior* zb)
	{
		if (zb)
		{		
			GroupBox^ gb = gcnew GroupBox();
			gb->Text = TO_MANAGED_STRING(zb->GetBehaviorTypeStr());
			gb->Width = 298;
			gb->Height = 20;
			const std::vector<ZobBehavior::wrapperData>* v = zb->GetWrappedVariables();
			if (v->size() > 0)
			{
				TableLayoutPanel^ panel = gcnew TableLayoutPanel();	
				panel->Width = 296;
				panel->ColumnCount = 1;
				panel->RowCount = v->size() + 1;
				//panel->Dock = DockStyle::Fill;
				//panel->BackColor = Drawing::Color::Green;
				panel->Height = v->size() * 35;
				TableLayoutColumnStyleCollection^ styles = panel->ColumnStyles;
				styles->Add(gcnew ColumnStyle(SizeType::Absolute, 300));
				//add subpanel for some space
				TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
				subPanel->Width = 300;
				subPanel->Height = 23;
				panel->Controls->Add(subPanel);
				int idx = 1;
				for (std::vector<ZobBehavior::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
				{
					ZobBehavior::wrapperData w = (*iter);
					if (w.type == ZobBehavior::eWrapperType_float)
					{
						AddFloatVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_enum)
					{
						AddEnumVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_bool)
					{
						AddBoolVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_string)
					{
						AddStringVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_int)
					{
						AddIntVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_ZobVector2)
					{
						AddZobVector2Variable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_ZobVector3)
					{
						AddZobVector3Variable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_zobId)
					{
						AddZobIdVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_zobObject)
					{
						AddZobObjectVariable(panel, &w);
					}
					else if (w.type == ZobBehavior::eWrapperType_path)
					{
						AddPathVariable(panel, &w);
					}
					idx++;
				}
				gb->Controls->Add(panel);
				gb->Height = panel->Height + 2;
				panel->Dock = DockStyle::Fill;	
				Button^ btn = gcnew Button();
				btn->Width = 20;
				btn->Height = 20;
				btn->Text = "+";
				btn->Top = 0;
				btn->Left = gb->Width - 20;
				btn->Click += gcnew EventHandler(this, &ZobObjectWrapper::GroupClick);
				gb->Controls->Add(btn);
			}
			return gb;
		}
		return nullptr;
	}

	void ZobObjectWrapper::AddPathVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		std::string s;
		std::string* name = (std::string*)(w->ptr);
		std::string* path = (std::string*)(w->ptr_1);
		std::string* file = (std::string*)(w->ptr_2);
		s = s.append(*name).append(";").append(*file).append(";").append(*path);
		txt->Text = TO_MANAGED_STRING(s.c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::PathHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddBoolVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		CheckBox^ cb = gcnew CheckBox();
		cb->Name = TO_MANAGED_STRING(w->internalName.c_str());
		bool* b = (bool*)(w->ptr);
		cb->Checked= (*b);
		cb->CheckedChanged += gcnew EventHandler(this, &ZobObjectWrapper::BoolHandler);
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(cb);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddEnumVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 25;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		ComboBox^ list = gcnew ComboBox();
		list->Name = TO_MANAGED_STRING(w->internalName.c_str());
		list->Width = 140;
		list->Height = 22;
		List<ComboboxItem^>^ items = gcnew List<ComboboxItem^>();
		for (int i = 0; i < w->enumNames.size(); i++)
		{
			String^ s = TO_MANAGED_STRING(w->enumNames[i].c_str());
			int idx = w->enumValues[i];
			ComboboxItem^ it = gcnew ComboboxItem(idx, s, w);
			items->Add(it);
		}
		list->BindingContext = gcnew BindingContext();
		list->DataSource = items;
		int* itemIdx = (int*)(w->ptr);
		list->SelectedIndex = 0;
		for (int i = 0; i < list->Items->Count; i++)
		{
			ComboboxItem^ it = (ComboboxItem^)list->Items[i];
			if (it->Key == *itemIdx)
			{
				list->SelectedIndex = i;
				break;
			}
		}
		list->SelectedIndexChanged += gcnew EventHandler(this, &ZobObjectWrapper::ListValidationHandler);
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(list);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddFloatVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		float* f = (float*)(w->ptr);
		txt->Text = (*f).ToString();
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::FloatHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddZobVector3Variable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		ZobVector3* v = (ZobVector3*)(w->ptr);
		txt->Text = TO_MANAGED_STRING(v->ToString().c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::ZobVector3Handler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddZobVector2Variable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		ZobVector2* v = (ZobVector2*)(w->ptr);
		txt->Text = TO_MANAGED_STRING(v->ToString().c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::ZobVector2Handler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddStringVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		std::string* s = (std::string*)(w->ptr);
		txt->Text = TO_MANAGED_STRING(s->c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::StringHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddIntVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		int* i = (int*)(w->ptr);
		txt->Text = (*i).ToString();
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::IntHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddZobIdVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		unsigned long long* i = (unsigned long long*)(w->ptr);
		txt->Text = (*i).ToString();
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	void ZobObjectWrapper::AddZobObjectVariable(TableLayoutPanel^ panel, ZobBehavior::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->Width = 300;
		subPanel->Height = 23;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		//label->Dock = DockStyle::Fill;
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		//txt->MaximumSize = Drawing::Size(100, 20);
		ZobObject** pz = (ZobObject**)(w->ptr);
		ZobObject* z = (ZobObject*)(*pz);
		if (z)
		{
			txt->Text = TO_MANAGED_STRING(z->GetName().c_str());
		}
		else
		{
			txt->Text = "";
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		panel->Controls->Add(subPanel);
	}

	const ZobBehavior::wrapperData* ZobObjectWrapper::GetWrapperDataForVariable(String^ variableName)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			const std::vector<ZobBehavior*>* behaviors = z->GetBehaviors();
			if(behaviors)
			{
				for (int i = 0; i < behaviors->size(); i++)
				{
					ZobBehavior* b = behaviors->at(i);
					if (b)
					{
						const std::vector<ZobBehavior::wrapperData>* v = b->GetWrappedVariables();
						if (v->size() > 0)
						{
							for (std::vector<ZobBehavior::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
							{
								const ZobBehavior::wrapperData* w = &(*iter);
								String^ wName = TO_MANAGED_STRING(w->internalName.c_str());
								if (wName == variableName)
								{
									return w;
								}
							}
						}
					}
				}
			}
		}
		return NULL;
	}

	void ZobObjectWrapper::GroupClick(Object^ sender, EventArgs^ e)
	{
		Button^ btn = (Button^)sender;
		GroupBox^ gb = (GroupBox^)btn->Parent;
		if (gb->Height != 25)
		{
			gb->Height = 25;
			btn->Text = "+";
		}
		else
		{
			int h = 0;
			for (int i = 0; i < gb->Controls->Count; i++)
			{
				h += gb->Controls[i]->Height + 2;
			}
			gb->Height = h;
			btn->Text = "-";
		}
	}

	void ZobObjectWrapper::ListValidationHandler(Object^ sender, System::EventArgs^ e)
	{
		ComboBox^ t = static_cast<ComboBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			ComboboxItem^ it = (ComboboxItem^)t->SelectedItem;
			if (it)
			{
				int* i = (int*)w->ptr;
				*i = it->Key;
			}
		}
	}

	void ZobObjectWrapper::BoolHandler(Object^ sender, EventArgs^ e)
	{
		CheckBox^ cb = static_cast<CheckBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(cb->Name);
		if (w)
		{
			try
			{
				bool* b = (bool*)w->ptr;
				cb->Checked = *b;
			}
			catch (...)
			{
				cb->Checked = false;
			}
		}
	}

	void ZobObjectWrapper::FloatHandler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			try
			{
				float* f = (float*)w->ptr;
				t->Text = t->Text->Replace(".", ",");
				float rez = (float)(Convert::ToDouble(t->Text));
				*f = rez;
			}
			catch (...)
			{
				float* f = (float*)w->ptr;
				t->Text = (*f).ToString();
			}
		}
	}

	void ZobObjectWrapper::IntHandler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			try
			{
				int* i = (int*)w->ptr;
				int rez = (float)(Convert::ToInt32(t->Text));
				*i = rez;
			}
			catch (...)
			{
				int* i = (int*)w->ptr;
				t->Text = (*i).ToString();
			}
		}
	}

	void ZobObjectWrapper::StringHandler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			std::string* internalString = (std::string*)w->ptr;
			std::string s;
			MarshalString(t->Text, s);
			internalString->assign(s);
		}
	}

	void ZobObjectWrapper::PathHandler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			std::string* name = (std::string*)w->ptr;
			std::string* path = (std::string*)w->ptr_1;
			std::string* file = (std::string*)w->ptr_2;
			cli::array<String^>^ a = t->Text->Split(';');
			std::string s;
			MarshalString(a[0], s);
			name->assign(s);
			MarshalString(a[1], s);
			path->assign(s);
			MarshalString(a[2], s);
			file->assign(s);
		}
	}

	void ZobObjectWrapper::ZobVector3Handler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			ZobVector3 v;
			std::string s;
			MarshalString(t->Text, s);
			if (v.FromString(s))
			{
				ZobVector3* vv = (ZobVector3*)w->ptr;
				vv->x = v.x;
				vv->y = v.y;
				vv->z = v.z;
			}
		}
	}

	void ZobObjectWrapper::ZobVector2Handler(Object^ sender, EventArgs^ e)
	{
		TextBox^ t = static_cast<TextBox^>(sender);
		const ZobBehavior::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			ZobVector2 v;
			std::string s;
			MarshalString(t->Text, s);
			if (v.FromString(s))
			{
				ZobVector2* vv = (ZobVector2*)w->ptr;
				vv->x = v.x;
				vv->y = v.y;
			}
		}
	}

	bool ZobObjectWrapper::IsFromFactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->IsFromFactoryFile();
		}
		return nullptr;
	}
	String^ ZobObjectWrapper::FactoryFile()
	{ 
		ZobObject* z = GetInstance();
		if (z)
		{
			return TO_MANAGED_STRING(z->FactoryFile().c_str());
		}
		return nullptr;
	}

	void ZobObjectWrapper::SaveToFactoryFile(String^ file)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string f;
			MarshalString(file, f);
			return z->SaveToFactoryFile(f);
		}
	}

	String^ ZobObjectWrapper::GetName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return TO_MANAGED_STRING(z->GetName().c_str());
		}
		return nullptr;
	}

	String^ ZobObjectWrapper::GetFullNodeName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string s;
			z->GetFullNodeName(s);
			return TO_MANAGED_STRING(s.c_str());
		}
		return nullptr;
	}

	String^ ZobObjectWrapper::GetMeshName()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return TO_MANAGED_STRING(z->GetMeshName().c_str());
		}
		return nullptr;
	}

	ManagedRenderOptions^ ZobObjectWrapper::GetRenderOptions()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedRenderOptions^ r = gcnew CLI::ManagedRenderOptions(z->GetRenderOptions());
			return r;
		}
		return nullptr;
	}

	void ZobObjectWrapper::SetMesh(String^ name)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			z->SetMesh(n);
		}
	}

	void ZobObjectWrapper::LoadMesh(String^ name, String^ file, String^ path)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			std::string f;
			MarshalString(file, f);
			std::string p;
			MarshalString(path, p);
			z->LoadMesh(n, f, p);
		}
	}

	void ZobObjectWrapper::CreateSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			z->CreateSprite();
		}
	}

	void ZobObjectWrapper::SetName(String^ name)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			std::string n;
			MarshalString(name, n);
			z->SetName(n);
		}
	}

	ManagedVector3^ ZobObjectWrapper::GetWorldPosition()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetWorldPosition());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetLocalPosition()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetLocalPosition());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetWorldRotation()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetWorldRotation());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetLocalRotation()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetLocalRotation());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetLeft()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetLeft());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetUp()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetUp());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetForward()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetForward());
			return v;
		}
		return nullptr;
	}

	ManagedVector3^ ZobObjectWrapper::GetScale()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ManagedVector3^ v = gcnew CLI::ManagedVector3(z->GetScale());
			return v;
		}
		return nullptr;
	}

	void ZobObjectWrapper::SetLocalTransform(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetLocalPosition(v.x, v.y, v.z);
		}
	}

	void ZobObjectWrapper::LookAt(ManagedVector3^ forward, ManagedVector3^ left, ManagedVector3^ up, bool addToCurrentRotation)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 l = left->ToVector3();
			ZobVector3 f = forward->ToVector3();
			ZobVector3 u = up->ToVector3();
			z->LookAt(&f, &l, &u, addToCurrentRotation);
		}
	}

	void ZobObjectWrapper::SetLocalRotation(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetLocalRotation(v.x, v.y, v.z);
		}
	}

	void ZobObjectWrapper::SetScale(ManagedVector3^ p)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZobVector3 v = p->ToVector3();
			z->SetScale(v.x, v.y, v.z);
		}
	}

	int ZobObjectWrapper::GetLightingMode()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetLightingMode();
		}
		return -1;
	}

	void ZobObjectWrapper::SetLightingMode(int lightMode)
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			RenderOptions::eLightMode l = (RenderOptions::eLightMode)lightMode;
			z->SetLightingMode(l);
		}
	}

	bool ZobObjectWrapper::IsLight()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			ZOBGUID::ZobSubType t = z->GetSubType();
			return t == ZOBGUID::ZobSubType::subtype_zobLight;
		}
		return false;
	}

	bool ZobObjectWrapper::IsCamera()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::ZobSubType::subtype_zobCamera;
		}
		return false;
	}

	bool ZobObjectWrapper::IsSprite()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			return z->GetSubType() == ZOBGUID::subtype_sprite;
		}
		return false;
	}

	bool ZobObjectWrapper::HasMesh()
	{
		ZobObject* z = GetInstance();
		if (z->GetMesh())
		{
			return true;
		}
		return false;
	}

	List<ZobObjectWrapper^>^ ZobObjectWrapper::GetChildren()
	{
		ZobObject* zo = GetInstance();
		try
		{
			if (zo)
			{
				List<ZobObjectWrapper^>^ list = gcnew List<ZobObjectWrapper^>();
				const std::vector<ZobObject*>* v = zo->GetChildren();
				for (std::vector<ZobObject*>::const_iterator iter = v->begin(); iter != v->end(); iter++)
				{
					ZobObject* z=*iter;
					if (dynamic_cast<Light*>(z))
					{
						list->Add(gcnew ZobLightWrapper(dynamic_cast<Light*>(z)));
					}
					else if (dynamic_cast<Camera*>(z))
					{
						list->Add(gcnew ZobCameraWrapper(dynamic_cast<Camera*>(z)));
					}
					else
					{
						list->Add(gcnew ZobObjectWrapper(z));
					}
				}
				return list;
			}
		}
		catch (...)
		{
			m_Instance = NULL;
			return nullptr;
		}
		return nullptr;
	}

	bool ZobObjectWrapper::IsValid()
	{
		if (m_Instance)
		{		
			if (DirectZob::GetInstance()->GetZobObjectManager()->IsDeleted(m_id))
			{
				m_Instance = NULL;
				return false;
			}
			else
			{
				return true;
			}
		}
		return false;
	}
}

#endif //_WINDLL