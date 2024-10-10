#include "DialogsInterface.h"


NewObjectDialogInterface::NewObjectDialogInterface(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
	NewObjectDialog(parent, id, title, pos, size, style)
{

}

void NewObjectDialogInterface::OnOK(wxCommandEvent& event)
{
	EndModal(wxOK);
}

void NewObjectDialogInterface::OnCancel(wxCommandEvent& event)
{ 
	EndModal(wxCANCEL);
}