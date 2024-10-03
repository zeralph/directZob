#include "Inspector.h"
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include "MainWindowInterface.h"
#include <string>

wxSize Inspector::sLabelSize = wxSize(80, 20);
wxSize Inspector::sFloatSize = wxSize(50, 20);
wxSize Inspector::sBoolSize = wxSize(250, 20);
wxSize Inspector::sStringSize = wxSize(100, 20);

bool Inspector::FloatToString(float f, std::string& s)
{
	s = to_string(f).c_str();
	s.erase(s.find_last_not_of('0') + 1, std::string::npos);
	s.erase(s.find_last_not_of('.') + 1, std::string::npos);
	return true;
}

bool Inspector::StringToFloat(const char* c, float& f)
{
	char* endptr;
	float result = strtof(c, &endptr);
	if (*endptr != '\0') {
		return false;
	}
	f = result;
	return true;
}

Inspector::Inspector(wxPanel* p)
{
	m_panel = p;
}

void Inspector::Clear()
{
	for (std::vector<ZobControl*>::iterator iter = m_controls.begin(); iter != m_controls.end(); iter++)
	{
		delete (*iter);
	}
	m_controls.clear();
	m_panel->DestroyChildren();
}

void Inspector::UpdateControls()
{
	for (std::vector<ZobControl*>::iterator iter = m_controls.begin(); iter != m_controls.end(); iter++)
	{
		(*iter)->Update();
	}
}

void Inspector::Set(ZobVariablesExposer* vars)
{
	Clear();
	m_panel->Hide();
	std::vector<wxStaticBoxSizer*> sizers;
	wxStaticBoxSizer* boxSizer = new wxStaticBoxSizer(new wxStaticBox(m_panel, wxID_ANY, _("label")), wxVERTICAL);
	sizers.push_back(boxSizer);
	if (vars)
	{
		const std::vector<ZobVariablesExposer::wrapperData>* v = vars->GetWrappedVariables();
		for (std::vector<ZobVariablesExposer::wrapperData>::const_iterator iter = v->begin(); iter != v->end(); iter++)
		{
			const ZobVariablesExposer::wrapperData* w = &(*iter);
			if (w->bVisible)
			{
				if (w->type == ZobVariablesExposer::eWrapperType_path)
				{
					//panel->Controls->Add(gcnew ZobControlFilePath(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_string)
				{
					wxBoxSizer* b = sizers.back();
					ZobStringCtrl* f = new ZobStringCtrl(w, b, m_panel);
					m_controls.push_back(f);
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_ZobVector3)
				{
					wxBoxSizer* b = sizers.back();
					ZobVector3Ctrl* f = new ZobVector3Ctrl(w, b, m_panel);
					m_controls.push_back(f);
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_float)
				{
					wxBoxSizer* b = sizers.back();
					ZobFloatCtrl* f = new ZobFloatCtrl(w, b, m_panel);
					m_controls.push_back(f);
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_enum)
				{
					wxBoxSizer* b = sizers.back();
					ZobEnumCtrl* f = new ZobEnumCtrl(w, b, m_panel);
					m_controls.push_back(f);
					//panel->Controls->Add(gcnew ZobControlEnum(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_enumCombo)
				{
					//panel->Controls->Add(gcnew ZobControlEnumCombo(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_bool)
				{
					wxBoxSizer* b = sizers.back();
					ZobBoolCtrl* f = new ZobBoolCtrl(w, b, m_panel);
					m_controls.push_back(f);
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_int)
				{
					//panel->Controls->Add(gcnew ZobControlFloat(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_startGroup)
				{
					wxStaticBoxSizer* newSizer = new wxStaticBoxSizer(new wxStaticBox(m_panel, wxID_ANY, _(w->name.c_str())), wxVERTICAL);
					sizers.back()->Add(newSizer, 0, wxALL, 1);				
					sizers.push_back(newSizer);
					//ZobGroupBox^ zb = gcnew ZobGroupBox(s, nullptr, false);
					//ZobPropertiesContainer^ p = gcnew ZobPropertiesContainer();
					//zb->Controls->Add(p);
					//panel->Controls->Add(zb);
					//System::String^ s = gcnew String(w.name.c_str());
					//panel = p;
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_endGroup)
				{
					assert((sizers.size() > 1));
					sizers.pop_back();
					//ZobPropertiesContainer^ p = (ZobPropertiesContainer^)panel->Parent->Parent;
					//panel = p;
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_zobId)
				{
					//panel->Controls->Add(gcnew ZobControlZobId(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_zobEntity)
				{
					//panel->Controls->Add(gcnew ZobControlZobEntity(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_zobColor)
				{
					//panel->Controls->Add(gcnew ZobControlColor(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_uint)
				{
					//panel->Controls->Add(gcnew ZobControlFloat(w));
				}
				else if (w->type == ZobVariablesExposer::eWrapperType_action)
				{
					//panel->Controls->Add(gcnew ZobControlAction(w));
				}
				else
				{
					assert(NULL);
				}
			}
		}
	}
	sizers.clear();
	m_panel->SetSizerAndFit(boxSizer, false);
	m_panel->Layout();
	m_panel->Show();
	//m_panel->FitInside(); // ask the sizer about the needed size
	//m_panel->SetScrollRate(5, 5);
}

Inspector::~Inspector()
{
	//Clear();
}

void Inspector::CreateStringControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b)
{

}

void Inspector::CreateBoolControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b)
{
}

void Inspector::CreateFloatControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b)
{

}

void Inspector::CreateZobVector3Control(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b)
{

}