#include <wx/filedlg.h>
#include "ZobControls.h"
#include "Inspector.h"
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/clrpicker.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include "MainWindowInterface.h"

/*------------------------------------------------
*
*	ZobControl
*
*/
ZobControl::ZobControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : wxPanel(p)
{
	m_vars = w;
	m_panel = p;
}
ZobControl::~ZobControl()
{
}
void ZobControl::UpdateFromEngine()
{
}
/*------------------------------------------------
*
*	ZobFloatCtrl
*
*/
ZobFloatCtrl::ZobFloatCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	
	float* f = (float*)w->ptr;
	std::string s;
	Inspector::FloatToString(*f, s);
	m_f = new wxTextCtrl(p, wxID_ANY, s.c_str(), wxDefaultPosition, Inspector::sFloatSize, 0);
	bs->Add(m_f, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	b->Add(bs);
	m_f->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ZobFloatCtrl::OnInput), NULL, this);
}
void ZobFloatCtrl::OnInput(wxFocusEvent& event)
{
	//return;
	float f;
	const char* val = m_f->GetValue().c_str();
	if (Inspector::StringToFloat(val, f))
	{
		float* pf = (float*)m_vars->ptr;
		*pf = f;
		if (m_vars->callback)
		{
			m_vars->callback(m_vars->id);
		}
	}
	else
	{
		wxEventBlocker blockerX(m_f);
		std::string s;
		f = *(float*)m_vars->ptr;
		Inspector::FloatToString(f, s);
		m_f->SetValue(s);
	}
	event.Skip();
}
ZobFloatCtrl::~ZobFloatCtrl()
{

}
void ZobFloatCtrl::UpdateFromEngine()
{
	float f = *((float*)m_vars->ptr);
	if (m_value != f)
	{
		std::string s;
		Inspector::FloatToString(f, s);
		wxEventBlocker blockerX(m_f);
		m_value = f;
		m_f->SetValue(s);
	}
}
/*------------------------------------------------
*
*	ZobVector3Ctrl
*
*/
ZobVector3Ctrl::ZobVector3Ctrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bsv = new wxBoxSizer(wxVERTICAL);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	bsv->Add(t, 0, wxALL, 1);
	wxBoxSizer* bsh = new wxBoxSizer(wxHORIZONTAL);
	bsv->Add(bsh, 0, wxALL, 1);
	//wxStaticBoxSizer* bs = new wxStaticBoxSizer(new wxStaticBox(m_panel, wxID_ANY, _(w->name)), wxHORIZONTAL);
	ZobVector3* z = (ZobVector3*)w->ptr;
	//X
	m_x = z->x;
	std::string sx;
	Inspector::FloatToString(z->x, sx);
	m_tx = new wxTextCtrl(p, wxID_ANY, sx, wxDefaultPosition, Inspector::sFloatSize, 0);
	bsh->Add(m_tx, 0, wxALL, 1);
	//Y
	m_y = z->y;
	std::string sy;
	Inspector::FloatToString(z->y, sy);
	m_ty = new wxTextCtrl(p, wxID_ANY, sy, wxDefaultPosition, Inspector::sFloatSize, 0);
	bsh->Add(m_ty, 0, wxALL, 1);
	//Z
	m_z = z->z;
	std::string sz;
	Inspector::FloatToString(z->z, sz);
	m_tz = new wxTextCtrl(p, wxID_ANY, sz, wxDefaultPosition, Inspector::sFloatSize, 0);
	bsh->Add(m_tz, 0, wxALL, 1);
	b->Add(bsv);
	//m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnExit ), this, m_menuItem5->GetId());
	m_tx->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ZobVector3Ctrl::OnInputX), NULL, this);
	m_ty->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ZobVector3Ctrl::OnInputY), NULL, this);
	m_tz->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ZobVector3Ctrl::OnInputZ), NULL, this);
	//m_textCtrl18->Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame2::OnChar), NULL, this);
	//m_textCtrl18->Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame2::OnChar), NULL, this);
	//m_textCtrl18->Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame2::OnChar), NULL, this);
}
ZobVector3Ctrl::~ZobVector3Ctrl()
{

}
void ZobVector3Ctrl::UpdateFromEngine()
{
	ZobVector3* z = (ZobVector3*)m_vars->ptr;
	if (m_x != z->x)
	{
		wxEventBlocker blockerX(m_tx);
		m_x = z->x;
		std::string s;
		Inspector::FloatToString(z->x, s);
		m_tx->SetValue(s);
	}
	if (m_y != z->y)
	{
		wxEventBlocker blockerY(m_ty);
		m_y = z->y;
		std::string s;
		Inspector::FloatToString(z->y, s);
		m_ty->SetValue(s);
	}
	if (m_z != z->z)
	{
		wxEventBlocker blockerZ(m_tz);
		m_z = z->z;
		std::string s;
		Inspector::FloatToString(z->z, s);
		m_tz->SetValue(s);
	}
}
void ZobVector3Ctrl::OnInputX(wxFocusEvent& event)
{
	//return;
	float f;
	ZobVector3* v = (ZobVector3*)m_vars->ptr;
	const char* val = m_tx->GetValue().c_str();
	if (Inspector::StringToFloat(val, f))
	{
		v->x = f;
		if (m_vars->callback)
		{
			m_vars->callback(m_vars->id);
		}
	}
	else
	{
		wxEventBlocker blockerX(m_tx);
		std::string s;
		Inspector::FloatToString(v->x, s);
		m_tx->SetValue(s);
	}
	event.Skip();
}
void ZobVector3Ctrl::OnInputY(wxFocusEvent& event)
{
	float f;
	ZobVector3* v = (ZobVector3*)m_vars->ptr;
	if (Inspector::StringToFloat(m_ty->GetValue().c_str(), f))
	{
		v->y = f;
		if (m_vars->callback)
		{
			m_vars->callback(m_vars->id);
		}
	}
	else
	{
		wxEventBlocker blockerX(m_ty);
		std::string s;
		Inspector::FloatToString(v->y, s);
		m_ty->SetValue(s);
	}
	event.Skip();
}
void ZobVector3Ctrl::OnInputZ(wxFocusEvent& event)
{
	float f;
	ZobVector3* v = (ZobVector3*)m_vars->ptr;
	if (Inspector::StringToFloat(m_tz->GetValue().c_str(), f))
	{
		v->z = f;
		if (m_vars->callback)
		{
			m_vars->callback(m_vars->id);
		}
	}
	else
	{
		wxEventBlocker blockerX(m_tz);
		std::string s;
		Inspector::FloatToString(v->z, s);
		m_tz->SetValue(s);
	}
	event.Skip();
}
/*------------------------------------------------
*
*	ZobEnumCtrl
*
*/
ZobEnumCtrl::ZobEnumCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	
	float* f = (float*)w->ptr;
	std::string s;
	Inspector::FloatToString(*f, s);
	m_c = new wxChoice(p, wxID_ANY, wxDefaultPosition, Inspector::sComboSize, 0, NULL, 0);

	std::vector<std::string>::const_iterator iter;
	for (iter = w->enumNames.begin(); iter != w->enumNames.end(); iter++)
	{
		m_c->Append((*iter));
	}
	int* idx = (int*)m_vars->ptr;
	m_c->SetSelection(*idx);
	bs->Add(m_c, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	b->Add(bs);
	m_c->Connect(wxEVT_CHOICE, wxCommandEventHandler(ZobEnumCtrl::OnSelected), NULL, this);
}
ZobEnumCtrl::~ZobEnumCtrl()
{

}
void ZobEnumCtrl::UpdateFromEngine()
{

}
void ZobEnumCtrl::OnSelected(wxCommandEvent& event)
{ 
	int idx = m_c->GetSelection();
	int enumValue = m_vars->enumValues.at(idx);
	int* v = (int*)m_vars->ptr;
	*v = enumValue;
	if (m_vars->callback)
	{
		m_vars->callback(m_vars->id);
	}
}
/*------------------------------------------------
*
*	ZobStringCtrl
*
*/
ZobStringCtrl::ZobStringCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	
	std::string* s = (std::string*)w->ptr;
	m_value = std::string(s->c_str());
	m_t = new wxTextCtrl(p, wxID_ANY, s->c_str(), wxDefaultPosition, Inspector::sStringSize, 0);
	bs->Add(m_t, 0, wxALL, 1);
	b->Add(bs);
	bs->Add(t, 0, wxALL, 1);
	m_t->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(ZobStringCtrl::OnInput), NULL, this);
}

ZobStringCtrl::~ZobStringCtrl()
{

}

void ZobStringCtrl::UpdateFromEngine()
{
	std::string* s = (std::string*)m_vars->ptr;
	std::string v = std::string(s->c_str());
	if (v != m_value)
	{
		m_value = v;
		m_t->SetValue(v);
	}
}

void ZobStringCtrl::OnInput(wxFocusEvent& event)
{
	std::string* s = (std::string*)m_vars->ptr;
	std::string n = std::string(m_t->GetValue().c_str());
	s->assign(n);
}
/*------------------------------------------------
*
*	ZobBoolCtrl
*
*/
ZobBoolCtrl::ZobBoolCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	bool* bval = (bool*)w->ptr;
	m_b = new wxCheckBox(p, wxID_ANY, _(""), wxDefaultPosition, Inspector::sCheckBoxSize, 0);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	bs->Add(m_b, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	b->Add(bs);
	m_b->Connect(wxEVT_CHECKBOX, wxCommandEventHandler(ZobBoolCtrl::OnUpdate), NULL, this);
	m_b->SetValue(*bval);
}

ZobBoolCtrl::~ZobBoolCtrl()
{

}

void ZobBoolCtrl::UpdateFromEngine()
{
	bool bval = *((bool*)m_vars->ptr);
	if (bval != m_b->GetValue())
	{
		m_b->SetValue(bval);
	}
}

void ZobBoolCtrl::OnUpdate(wxCommandEvent& event)
{
	bool* pb = (bool*)m_vars->ptr;
	*pb = m_b->GetValue();
	if (m_vars->callback)
	{
		m_vars->callback(m_vars->id);
	}
}

void ZobBoolCtrl::OnInput(wxFocusEvent& event)
{

}

/*------------------------------------------------
*
*	ZobColorCtrl
*
*/
ZobColorControl::ZobColorControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	ZobColor* c = (ZobColor*)w->ptr;
	m_c = new wxColourPickerCtrl(p, wxID_ANY, *wxWHITE, wxDefaultPosition, Inspector::sColorSize);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	t->Wrap(-1);
	bs->Add(m_c, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	b->Add(bs);
	m_c->Connect(wxEVT_COLOURPICKER_CHANGED, wxCommandEventHandler(ZobColorControl::OnUpdate), NULL, this);
	wxColor wxc = wxColor();
	wxc.Set(c->GetRed(), c->GetGreen(), c->GetBlue(), c->GetAlpha());
	m_c->SetColour(wxc);
	//m_b->SetValue(*bval);
}
ZobColorControl::~ZobColorControl()
{
}
void ZobColorControl::UpdateFromEngine()
{
	ZobColor* c = (ZobColor*)m_vars->ptr;
	bool b = c->GetRed() != m_color.GetRed() || 
			c->GetGreen() != m_color.GetGreen() || 
			c->GetBlue() != m_color.GetBlue() || 
			c->GetAlpha() != m_color.GetAlpha();
	if(b)
	{
		m_color = c;
		wxColor wxc = wxColor();
		wxc.Set(c->GetRed(), c->GetGreen(), c->GetBlue(), c->GetAlpha());
		m_c->SetColour(wxc);
	}
}
void ZobColorControl::OnUpdate(wxCommandEvent& event)
{
	wxColor wxc = m_c->GetColour();
	ZobColor* c = (ZobColor*)m_vars->ptr;
	c->Set(wxc.GetAlpha(), wxc.GetRed(), wxc.GetGreen(), wxc.GetBlue());
	//*pb = m_c->GetValue();
	if (m_vars->callback)
	{
		m_vars->callback(m_vars->id);
	}
}

/*------------------------------------------------
*
*	ZobEntityCtrl
*
*/
ZobEntityControl::ZobEntityControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	int style = 0;
	if (w->bReadOnly)
	{
		style = wxTE_READONLY;
	}
	m_t = new wxTextCtrl(p, wxID_ANY, "", wxDefaultPosition, Inspector::sStringSize, style);
	DirectZobType::zobId* zid = (DirectZobType::zobId*)w->ptr;
	std::stringstream ss;
	ss << *zid;
	m_t->SetValue(ss.str());
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	bs->Add(m_t, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	b->Add(bs);
}
ZobEntityControl::~ZobEntityControl()
{
}
void ZobEntityControl::UpdateFromEngine()
{
}
void ZobEntityControl::OnUpdate(wxCommandEvent& event)
{
	if (m_vars->callback)
	{
		m_vars->callback(m_vars->id);
	}
}

/*------------------------------------------------
*
*	ZobPathCtrl
*
*/
ZobPathControl::ZobPathControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p) : ZobControl(w, b, p)
{
	wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
	std::string* c = (std::string*)w->ptr;
	m_t = new wxTextCtrl(p, wxID_ANY, "", wxDefaultPosition, Inspector::sStringSize, wxTE_READONLY);
	m_t->SetValue(*c);
	wxStaticText* t = new wxStaticText(p, wxID_ANY, _(w->name.c_str()), wxDefaultPosition, Inspector::sLabelSize, 0);
	wxButton* btn = new wxButton(p, wxID_ANY, _("..."));
	t->Wrap(-1);
	bs->Add(m_t, 0, wxALL, 1);
	bs->Add(btn, 0, wxALL, 1);
	bs->Add(t, 0, wxALL, 1);
	btn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ZobPathControl::OnOpen), NULL, this);
	b->Add(bs);
}
void ZobPathControl::OnOpen(wxCommandEvent& event)
{
	ZobFilePath* zfp = (ZobFilePath*)m_vars->ptr;
	const char* ft = ZOB_ANY_EXT;
	if (zfp)
	{
		switch (zfp->GetFileType())
		{
		case ZobFilePath::eFileType_asset:
			ft = ZOB_ASSET_EXT;
			break;
		case ZobFilePath::eFileType_image:
			ft = ZOB_IMG_EXT;
			break;
		case ZobFilePath::eFileType_mesh:
			ft = ZOB_MESH_EXT;
			break;
		case ZobFilePath::eFileType_scene:
			ft = ZOB_SCENE_EXT;
			break;
		case ZobFilePath::eFileType_xml:
			ft = ZOB_XML_EXT;
			break;
		default:
			break;
		}
	}
	wxFileDialog openFileDialog(this, _("DirectZob Scene"), "", "", ft, wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	m_path = std::string(openFileDialog.GetDirectory().mb_str()) + '\\';
	m_file = std::string(openFileDialog.GetFilename().mb_str());
	m_fullPath = m_path + m_file;
	zfp->ChangePath(m_path, m_file);
	if (m_vars->callback)
	{
		m_vars->callback(m_vars->id);
	}
}
ZobPathControl::~ZobPathControl()
{
}
void ZobPathControl::UpdateFromEngine()
{
	ZobFilePath* zfp = (ZobFilePath*)m_vars->ptr;
	if (zfp->GetFullPath() != m_fullPath)
	{
		m_fullPath = zfp->GetFullPath();
		m_t->SetValue(m_fullPath);
	}
}
void ZobPathControl::OnUpdate(wxCommandEvent& event)
{

}