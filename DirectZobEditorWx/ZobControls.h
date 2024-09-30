#pragma once
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Misc/ZobVariablesExposer.h"
#include <wx/panel.h>

class wxTextCtrl;
class wxBoxSizer;
class wxComboBox;
class ZobControl : public wxPanel
{
public:
	ZobControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	virtual ~ZobControl();
	virtual void Update() = 0;
protected:
	const ZobVariablesExposer::wrapperData* m_vars;
	wxBoxSizer* m_boxSizer;
	wxPanel* m_panel;
};

class ZobFloatCtrl : public ZobControl
{
public:
	ZobFloatCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobFloatCtrl() override;
	void Update() override;
	void OnInput(wxFocusEvent& event);
private:
	wxTextCtrl* m_f;
	float m_value;
};

class ZobVector3Ctrl : public ZobControl
{
public:
	ZobVector3Ctrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobVector3Ctrl() override;
	void Update() override;
	void OnInputX(wxFocusEvent& event);
	void OnInputY(wxFocusEvent& event);
	void OnInputZ(wxFocusEvent& event);
	//void OnEnterX(wxCommandEvent& event);
	//void OnEnterY(wxCommandEvent& event);
	//void OnEnterZ(wxCommandEvent& event);
private:
	wxTextCtrl* m_tx;
	wxTextCtrl* m_ty;
	wxTextCtrl* m_tz;
	float m_x;
	float m_y;
	float m_z;
};

class ZobEnumCtrl : public ZobControl
{
public:
	ZobEnumCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobEnumCtrl() override;
	void Update() override;
	void OnInput(wxFocusEvent& event);
	void OnSelected(wxCommandEvent& event);
private:
	wxComboBox* m_c;
};
