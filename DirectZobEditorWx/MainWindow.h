///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/treectrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/toolbar.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/notebook.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainWindow
///////////////////////////////////////////////////////////////////////////////
class MainWindow : public wxFrame
{
	private:

	protected:
		wxSplitterWindow* m_splitter1;
		wxPanel* m_panel1;
		wxMenu* m_treeMenu;
		wxPanel* m_panel2;
		wxSplitterWindow* m_splitter3;
		wxPanel* m_panel3;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_panel7;
		wxToolBar* m_toolBar1;
		wxToggleButton* m_Play;
		wxToggleButton* m_Pause;
		wxToggleButton* m_Stop;
		wxComboBox* m_cameraSelector;
		wxToggleButton* m_WT;
		wxToggleButton* m_WR;
		wxToggleButton* m_LT;
		wxToggleButton* m_LR;
		wxButton* m_Snap;
		wxPanel* m_panel10;
		wxPanel* m_panelRight;
		wxNotebook* m_notebookInspector;
		wxPanel* m_panelInspector;
		wxPanel* m_panelEngine;
		wxPanel* m_panelScene;
		wxMenuBar* m_menubar1;
		wxMenu* m_menu1;
		wxMenu* m_gizmosMenu;
		wxStatusBar* m_statusBar1;

		// Virtual event handlers, override them in your derived class
		virtual void OnExit( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnMaximize( wxMaximizeEvent& event ) { event.Skip(); }
		virtual void OnStartDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnEndDrag( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnTreeRightClick( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnTreeSelChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuAddComponent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuZoom( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuDuplicate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuShowHide( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTreeMenuAddObject( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlay( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPause( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCameraSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnModificatorClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSnap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMouseClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMotion( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMyPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGizmosGrid( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGizmosText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGizmosPhysics( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGizmosObjects( wxCommandEvent& event ) { event.Skip(); }


	public:
		wxTreeCtrl* m_treeNode;
		wxPanel* m_renderPanel;
		wxTextCtrl* m_log;

		MainWindow( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1397,762 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MainWindow();

		void m_splitter1OnIdle( wxIdleEvent& )
		{
			m_splitter1->SetSashPosition( 250 );
			m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter1OnIdle ), NULL, this );
		}

		void m_splitter3OnIdle( wxIdleEvent& )
		{
			m_splitter3->SetSashPosition( -350 );
			m_splitter3->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter3OnIdle ), NULL, this );
		}

		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( -150 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter4OnIdle ), NULL, this );
		}

};

