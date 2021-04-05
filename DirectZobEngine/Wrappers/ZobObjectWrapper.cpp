#ifdef _WINDLL
#include "DirectZobWrapper.h"
#include "ZobObjectWrapper.h"
#include "ZobCameraWrapper.h"
#include "ZobLightWrapper.h"
#include "../ZobObjects/Light.h"
#include "../ZobObjects/Camera.h"
#include "../DirectZob.h"
#include "../Misc/ZobVariablesExposer.h"

#define TO_MANAGED_STRING(x) gcnew String(x);

namespace CLI
{
	ZobObjectWrapper::ZobObjectWrapper(ZobObject* zobObject, Panel^ panel):ManagedObject(zobObject, false)
	{
		m_zobObject = zobObject;
		m_id = zobObject->GetIdValue();
		m_panel = panel;
		m_objectPanel = nullptr;
		m_container = nullptr;
		CreateObjectview();
		CreateObjectBeahaviorsview();
	}

	ZobObjectWrapper::~ZobObjectWrapper()
	{
		m_panel->Controls->Remove(m_objectPanel);
		m_Instance = NULL;
	}

	void ZobObjectWrapper::CreateObjectview()
	{
		String^ mStr;
		ZobControlString^ s;
		m_objectPanel = gcnew ZobGroupBox("Object", false);
		m_container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ properties = gcnew ZobGroupBox("Properties", false);
		ZobPropertiesContainer^ container = gcnew ZobPropertiesContainer();
		ZobGroupBox^ objectgb = FillObjectControl(m_zobObject);
		m_container->Controls->Add(objectgb);
		m_objectPanel->Controls->Add(m_container);
		m_panel->Controls->Add(m_objectPanel);
	}

	void ZobObjectWrapper::CreateObjectBeahaviorsview()
	{
		ZobObject* z = GetInstance();
		if (z)
		{
			const std::vector<ZobBehavior*>* behaviors = z->GetBehaviors();
			if (behaviors->size() > 0)
			{
				for (int i = 0; i < behaviors->size(); i++)
				{
					ZobBehavior* zb = behaviors->at(i);
					ZobGroupBox^ behaviorPanel = FillBehaviorControl(zb);
					m_container->Controls->Add(behaviorPanel);
				}
			}
		}
	}

	ZobGroupBox^ ZobObjectWrapper::FillObjectControl(ZobObject* z)
	{
		if (z)
		{
			std::string name = z->GetName();
			ZobVariablesExposer* v = z->GetVariablesExposer();
			return FillControlInternal(name, v);
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillBehaviorControl(ZobBehavior* zb)
	{
		if (zb)
		{		
			std::string name = zb->GetBehaviorTypeStr();
			ZobVariablesExposer* v = zb->GetVariablesExposer();
			return FillControlInternal(name, v);
		}
		return nullptr;
	}

	ZobGroupBox^ ZobObjectWrapper::FillControlInternal(std::string& name, ZobVariablesExposer* ze)
	{
		String^ panelName = TO_MANAGED_STRING(name.c_str());
		ZobGroupBox^ zobPanel = gcnew ZobGroupBox(panelName, true);
		const std::vector<ZobVariablesExposer::wrapperData>* v = ze->GetWrappedVariables();
		if (v->size() > 0)
		{
			ZobPropertiesContainer^ panel = gcnew ZobPropertiesContainer();
			int idx = 1;
			for (std::vector<ZobVariablesExposer::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
			{
				ZobVariablesExposer::wrapperData w = (*iter);
				if (w.type == ZobVariablesExposer::eWrapperType_float)
				{
					panel->Controls->Add(AddFloatVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_enum)
				{
					panel->Controls->Add(AddEnumVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_bool)
				{
					panel->Controls->Add(AddBoolVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_string)
				{
					panel->Controls->Add(AddStringVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_int)
				{
					panel->Controls->Add(AddIntVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_ZobVector2)
				{
					panel->Controls->Add(AddZobVector2Variable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_ZobVector3)
				{
					panel->Controls->Add(AddZobVector3Variable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_zobId)
				{
					panel->Controls->Add(AddZobIdVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_zobObject)
				{
					panel->Controls->Add(AddZobObjectVariable(&w));
				}
				else if (w.type == ZobVariablesExposer::eWrapperType_path)
				{
					panel->Controls->Add(AddPathVariable(&w));
				}
				idx++;
			}
			zobPanel->Controls->Add(panel);
		}
		return zobPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddPathVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
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
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddBoolVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
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
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddEnumVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
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
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddFloatVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		float* f = (float*)(w->ptr);
		txt->Text = (*f).ToString();
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::FloatHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		return subPanel;
	}

	ZobControlVector3^ ZobObjectWrapper::AddZobVector3Variable(ZobVariablesExposer::wrapperData* w)
	{
		String^ internalName = TO_MANAGED_STRING(w->internalName.c_str());
		String^ name = TO_MANAGED_STRING(w->name.c_str());
		ZobVector3* v = (ZobVector3*)(w->ptr);
		ZobControlVector3^ ctrl = gcnew ZobControlVector3(internalName, name, v, false, nullptr);
		ctrl->OnChangeEvent += gcnew ZobControlVector3::OnChange(this, &ZobObjectWrapper::ZobVector3Handler);
		DirectZobWrapper::OnEditorUpdateEvent += gcnew DirectZobWrapper::OnEditorUpdate(ctrl, &CLI::ZobControlVector3::UpdateZobControl);
		return ctrl;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddZobVector2Variable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		ZobVector2* v = (ZobVector2*)(w->ptr);
		txt->Text = TO_MANAGED_STRING(v->ToString().c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::ZobVector2Handler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddStringVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		std::string* s = (std::string*)(w->ptr);
		txt->Text = TO_MANAGED_STRING(s->c_str());
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::StringHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddIntVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		int* i = (int*)(w->ptr);
		txt->Text = (*i).ToString();
		if (!w->bReadOnly)
		{
			txt->Leave += gcnew EventHandler(this, &ZobObjectWrapper::IntHandler);
		}
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddZobIdVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
		unsigned long long* i = (unsigned long long*)(w->ptr);
		txt->Text = (*i).ToString();
		subPanel->Controls->Add(label);
		subPanel->Controls->Add(txt);
		return subPanel;
	}

	TableLayoutPanel^ ZobObjectWrapper::AddZobObjectVariable(ZobVariablesExposer::wrapperData* w)
	{
		TableLayoutPanel^ subPanel = gcnew TableLayoutPanel();
		subPanel->AutoSize = true;
		subPanel->ColumnCount = 2;
		subPanel->RowCount = 1;
		Label^ label = gcnew Label();
		label->Text = TO_MANAGED_STRING(w->name.c_str());
		label->Width = 140;
		label->Height = 20;
		label->TextAlign = ContentAlignment::BottomRight;
		TextBox^ txt = gcnew TextBox();
		txt->Name = TO_MANAGED_STRING(w->internalName.c_str());
		txt->Width = 140;
		txt->Height = 20;
		txt->ReadOnly = w->bReadOnly;
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
		return subPanel;
	}

	const ZobVariablesExposer::wrapperData* ZobObjectWrapper::GetWrapperDataForVariable(String^ variableName)
	{
		//TODO : use id to get the right object !
		ZobObject* z = GetInstance();
		if (z)
		{
			//search in object
			//
			const std::vector<ZobVariablesExposer::wrapperData>* v = z->GetVariablesExposer()->GetWrappedVariables();
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
			//search in behaviors
			const std::vector<ZobBehavior*>* behaviors = z->GetBehaviors();
			if(behaviors)
			{
				for (int i = 0; i < behaviors->size(); i++)
				{
					ZobBehavior* b = behaviors->at(i);
					if (b)
					{
						v = b->GetVariablesExposer()->GetWrappedVariables();
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(cb->Name);
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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

	void ZobObjectWrapper::EditorUpdate()
	{

	}

	void ZobObjectWrapper::ZobVector3Handler(Object^ sender, EventArgs^ e)
	{
		ZobControlVector3^ t = static_cast<ZobControlVector3^>(sender);
		assert(t);
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
		if (w)
		{
			ZobVector3 v;
			std::string sx;
			MarshalString(t->txt_X->Text, sx);
			std::string sy;
			MarshalString(t->txt_Y->Text, sy);
			std::string sz;
			MarshalString(t->txt_Z->Text, sz);
			if (v.FromString(sx, sy, sz))
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
		const ZobVariablesExposer::wrapperData* w = GetWrapperDataForVariable(t->Name);
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
		/*
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
		*/
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