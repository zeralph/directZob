///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MainWindow.h"

///////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	bSizer1->SetMinSize( wxSize( 150,-1 ) );
	m_splitter1 = new wxSplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter1->Connect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter1OnIdle ), NULL, this );
	m_splitter1->SetMinimumPaneSize( 150 );

	m_panel1 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel1->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_INACTIVECAPTION ) );
	m_panel1->SetMinSize( wxSize( 150,-1 ) );
	m_panel1->SetMaxSize( wxSize( 250,-1 ) );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_treeNode = new wxTreeCtrl( m_panel1, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTR_DEFAULT_STYLE );
	m_treeNode->SetMinSize( wxSize( 150,-1 ) );
	m_treeNode->SetMaxSize( wxSize( 250,-1 ) );

	bSizer2->Add( m_treeNode, 1, wxALL|wxEXPAND, 0 );


	m_panel1->SetSizer( bSizer2 );
	m_panel1->Layout();
	bSizer2->Fit( m_panel1 );
	m_panel2 = new wxPanel( m_splitter1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	bSizer4->SetMinSize( wxSize( 150,-1 ) );
	m_splitter3 = new wxSplitterWindow( m_panel2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter3->SetSashGravity( 1 );
	m_splitter3->Connect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter3OnIdle ), NULL, this );
	m_splitter3->SetMinimumPaneSize( 150 );

	m_panel3 = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	bSizer5->SetMinSize( wxSize( 150,-1 ) );
	m_splitter4 = new wxSplitterWindow( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	m_splitter4->SetSashGravity( 1 );
	m_splitter4->Connect( wxEVT_IDLE, wxIdleEventHandler( MainWindow::m_splitter4OnIdle ), NULL, this );
	m_splitter4->SetMinimumPaneSize( 150 );

	m_panel7 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );

	m_toolBar1 = new wxToolBar( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_Play = new wxToggleButton( m_toolBar1, wxID_ANY, _("Play"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Play );
	m_Pause = new wxToggleButton( m_toolBar1, wxID_ANY, _("Pause"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Pause );
	m_Stop = new wxToggleButton( m_toolBar1, wxID_ANY, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Stop );
	m_cameraSelector = new wxComboBox( m_toolBar1, wxID_ANY, _("Camera"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_toolBar1->AddControl( m_cameraSelector );
	m_toolBar1->Realize();

	bSizer6->Add( m_toolBar1, 0, wxEXPAND, 5 );

	m_renderPanel = new wxPanel( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_renderPanel->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) );

	bSizer6->Add( m_renderPanel, 1, wxEXPAND | wxALL, 1 );


	m_panel7->SetSizer( bSizer6 );
	m_panel7->Layout();
	bSizer6->Fit( m_panel7 );
	m_panel10 = new wxPanel( m_splitter4, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );

	m_log = new wxRichTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0|wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS );
	bSizer8->Add( m_log, 1, wxEXPAND | wxALL, 5 );


	m_panel10->SetSizer( bSizer8 );
	m_panel10->Layout();
	bSizer8->Fit( m_panel10 );
	m_splitter4->SplitHorizontally( m_panel7, m_panel10, -150 );
	bSizer5->Add( m_splitter4, 1, wxEXPAND, 5 );


	m_panel3->SetSizer( bSizer5 );
	m_panel3->Layout();
	bSizer5->Fit( m_panel3 );
	m_panelInspector = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTAB_TRAVERSAL );
	m_panelInspector->SetBackgroundColour( wxColour( 200, 0, 0 ) );
	m_panelInspector->SetMinSize( wxSize( 150,-1 ) );
	m_panelInspector->SetMaxSize( wxSize( 250,-1 ) );

	m_splitter3->SplitVertically( m_panel3, m_panelInspector, -250 );
	bSizer4->Add( m_splitter3, 1, wxEXPAND, 5 );


	m_panel2->SetSizer( bSizer4 );
	m_panel2->Layout();
	bSizer4->Fit( m_panel2 );
	m_splitter1->SplitVertically( m_panel1, m_panel2, 250 );
	bSizer1->Add( m_splitter1, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem1 );

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Open ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem2 );

	wxMenuItem* m_menuItem3;
	m_menuItem3 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Save as ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem3 );

	wxMenuItem* m_menuItem4;
	m_menuItem4 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Save") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem4 );

	wxMenuItem* m_menuItem5;
	m_menuItem5 = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItem5 );

	m_menubar1->Append( m_menu1, _("File") );

	m_menu2 = new wxMenu();
	m_menubar1->Append( m_menu2, _("MyMenu") );

	this->SetMenuBar( m_menubar1 );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_MAXIMIZE, wxMaximizeEventHandler( MainWindow::OnMaximize ) );
	m_treeNode->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainWindow::OnTreeSelChanged ), NULL, this );
	m_Play->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnPlay ), NULL, this );
	m_Pause->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnPause ), NULL, this );
	m_Stop->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnStop ), NULL, this );
	m_cameraSelector->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainWindow::OnCameraSelected ), NULL, this );
	m_renderPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainWindow::OnLeftDown ), NULL, this );
	m_renderPanel->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( MainWindow::OnMiddleDown ), NULL, this );
	m_renderPanel->Connect( wxEVT_MOTION, wxMouseEventHandler( MainWindow::OnMotion ), NULL, this );
	m_renderPanel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MainWindow::OnMouseWheel ), NULL, this );
	m_renderPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( MainWindow::OnMyPaint ), NULL, this );
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnNew ), this, m_menuItem1->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnOpen ), this, m_menuItem2->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnSaveAs ), this, m_menuItem3->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnSave ), this, m_menuItem4->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnExit ), this, m_menuItem5->GetId());
}

MainWindow::~MainWindow()
{
}
