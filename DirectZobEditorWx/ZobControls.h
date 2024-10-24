#pragma once
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Misc/ZobVariablesExposer.h"
#include "../DirectZobEngine/Rendering/Color.h"
#include <wx/panel.h>

class wxTextCtrl;
class wxBoxSizer;
class wxChoice;
class wxCheckBox;
class wxColourPickerCtrl;
class ZobControl : public wxPanel
{
public:
	ZobControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	virtual ~ZobControl();
	virtual void UpdateFromEngine()=0;
protected:
	const ZobVariablesExposer::wrapperData* m_vars;
	wxPanel* m_panel;
};

class ZobFloatCtrl : public ZobControl
{
public:
	ZobFloatCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobFloatCtrl() override;
	void UpdateFromEngine() override;
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
	void UpdateFromEngine() override;
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
	void UpdateFromEngine() override;
	void OnSelected(wxCommandEvent& event);
private:
	wxChoice* m_c;
};

class ZobStringCtrl : public ZobControl
{
public:
	ZobStringCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobStringCtrl() override;
	void UpdateFromEngine() override;
	void OnInput(wxFocusEvent& event);
private:
	wxTextCtrl* m_t;
	std::string m_value;
};

class ZobBoolCtrl : public ZobControl
{
public:
	ZobBoolCtrl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	~ZobBoolCtrl() override;
	void UpdateFromEngine() override;
	void OnUpdate(wxCommandEvent& event);
	void OnInput(wxFocusEvent& event);
private:
	wxCheckBox* m_b;
};

class ZobColorControl : public ZobControl
{
public:
	ZobColorControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	virtual ~ZobColorControl();
	void UpdateFromEngine() override;
	void OnUpdate(wxCommandEvent& event);
private:
	wxColourPickerCtrl* m_c;
	ZobColor m_color;
};

class ZobEntityControl : public ZobControl
{
public:
	ZobEntityControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	virtual ~ZobEntityControl();
	void UpdateFromEngine() override;
	void OnUpdate(wxCommandEvent& event);
private:
	wxTextCtrl* m_t;
	ZobObject* m_entity;
};

class ZobPathControl : public ZobControl
{
public:
	ZobPathControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b, wxPanel* p);
	virtual ~ZobPathControl();
	void UpdateFromEngine() override;
	void OnUpdate(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
private:
	wxTextCtrl* m_t;
	std::string m_path;
	std::string m_file;
	std::string m_fullPath;
};