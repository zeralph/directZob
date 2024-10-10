#pragma once
#include "../DirectZobEngine/DirectZob.h"
#include "../DirectZobEngine/Misc/ZobVariablesExposer.h"
#include <wx/panel.h>
#include <wx/sizer.h>
#include "ZobControls.h"


class Inspector
{
public:
	Inspector(wxPanel* p);
	~Inspector();
	void Set(ZobVariablesExposer* vars);
	void Set(ZobObject* z);
	void Clear();
	void UpdateControls();
	static bool StringToFloat(const char* c, float& f);
	static bool	FloatToString(float f, std::string& s);
private:
	void AddVars(ZobVariablesExposer* vars, std::vector<wxStaticBoxSizer*>* sizers);
	void CreateBoolControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b);
	void CreateFloatControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b);
	void CreateStringControl(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b);
	void CreateZobVector3Control(const ZobVariablesExposer::wrapperData* w, wxBoxSizer* b);
	wxPanel* m_parentPanel;
	wxPanel* m_panel;
	std::vector<ZobControl*> m_controls;
public:
	bool			m_dirty;
	static wxSize	sLabelSize;
	static wxSize	sFloatSize;
	static wxSize	sStringSize;
	static wxSize	sBoolSize;
	static wxSize	sColorSize;
	static wxSize	sComboSize;
	static wxSize	sCheckBoxSize;
	//wxBoxSizer* m_boxSizer;
};