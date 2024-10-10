#pragma once
#include "Dialogs.h"

class NewObjectDialogInterface : public NewObjectDialog
{
	private:
	protected:
		virtual void OnOK(wxCommandEvent& event)override;
		virtual void OnCancel(wxCommandEvent& event) override;
	public:
		NewObjectDialogInterface(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
		std::string GetName() { return std::string(m_newName->GetValue().c_str());  }
};

