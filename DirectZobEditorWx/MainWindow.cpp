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
	m_Play = new wxToggleButton( m_toolBar1, wxID_ANY, _("Play"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_toolBar1->AddControl( m_Play );
	m_Pause = new wxToggleButton( m_toolBar1, wxID_ANY, _("Pause"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_toolBar1->AddControl( m_Pause );
	m_Stop = new wxToggleButton( m_toolBar1, wxID_ANY, _("Stop"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_toolBar1->AddControl( m_Stop );
	m_toolBar1->AddSeparator();

	m_cameraSelector = new wxComboBox( m_toolBar1, wxID_ANY, _("Camera"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_toolBar1->AddControl( m_cameraSelector );
	m_toolBar1->AddSeparator();

	m_WT = new wxToggleButton( m_toolBar1, wxID_ANY, _("WT"), wxDefaultPosition, wxDefaultSize, 0 );
	m_WT->SetValue( true );
	m_toolBar1->AddControl( m_WT );
	m_WR = new wxToggleButton( m_toolBar1, wxID_ANY, _("WR"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_WR );
	m_LT = new wxToggleButton( m_toolBar1, wxID_ANY, _("LT"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_LT );
	m_LR = new wxToggleButton( m_toolBar1, wxID_ANY, _("LR"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_LR );
	m_Snap = new wxButton( m_toolBar1, wxID_ANY, _("Snap"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Snap );
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

	m_log = new wxTextCtrl( m_panel10, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_MULTILINE|wxTE_RICH );
	bSizer8->Add( m_log, 1, wxALL|wxEXPAND, 2 );


	m_panel10->SetSizer( bSizer8 );
	m_panel10->Layout();
	bSizer8->Fit( m_panel10 );
	m_splitter4->SplitHorizontally( m_panel7, m_panel10, -150 );
	bSizer5->Add( m_splitter4, 1, wxEXPAND, 5 );


	m_panel3->SetSizer( bSizer5 );
	m_panel3->Layout();
	bSizer5->Fit( m_panel3 );
	m_panelRight = new wxPanel( m_splitter3, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), wxTAB_TRAVERSAL );
	m_panelRight->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	m_panelRight->SetMinSize( wxSize( 150,-1 ) );
	m_panelRight->SetMaxSize( wxSize( 250,-1 ) );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );

	m_notebookInspector = new wxNotebook( m_panelRight, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_notebookInspector->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	m_notebookInspector->SetMinSize( wxSize( 250,-1 ) );

	m_panelInspector = new wxPanel( m_notebookInspector, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelInspector->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	m_notebookInspector->AddPage( m_panelInspector, _("Object"), true );
	m_panelEngine = new wxPanel( m_notebookInspector, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panelEngine->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );

	m_notebookInspector->AddPage( m_panelEngine, _("Engine"), false );
	m_panelScene = new wxPanel( m_notebookInspector, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_notebookInspector->AddPage( m_panelScene, _("Scene"), false );

	bSizer7->Add( m_notebookInspector, 1, wxEXPAND | wxALL, 0 );


	m_panelRight->SetSizer( bSizer7 );
	m_panelRight->Layout();
	m_splitter3->SplitVertically( m_panel3, m_panelRight, -250 );
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
	wxMenuItem* m_menuIItemNew;
	m_menuIItemNew = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("New") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuIItemNew );

	wxMenuItem* m_menuItemOpen;
	m_menuItemOpen = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Open ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemOpen );

	wxMenuItem* m_menuItemSaveAs;
	m_menuItemSaveAs = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Save as ...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemSaveAs );

	wxMenuItem* m_menuItemSave;
	m_menuItemSave = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Save") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemSave );

	wxMenuItem* m_menuItemExit;
	m_menuItemExit = new wxMenuItem( m_menu1, wxID_ANY, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu1->Append( m_menuItemExit );

	m_menubar1->Append( m_menu1, _("File") );

	m_gizmosMenu = new wxMenu();
	wxMenuItem* m_menuItemGrid;
	m_menuItemGrid = new wxMenuItem( m_gizmosMenu, wxID_ANY, wxString( _("Grid") ) , wxEmptyString, wxITEM_CHECK );
	m_gizmosMenu->Append( m_menuItemGrid );

	wxMenuItem* m_menuItemDebugText;
	m_menuItemDebugText = new wxMenuItem( m_gizmosMenu, wxID_ANY, wxString( _("Debug text") ) , wxEmptyString, wxITEM_CHECK );
	m_gizmosMenu->Append( m_menuItemDebugText );

	wxMenuItem* m_menuItemPhysics;
	m_menuItemPhysics = new wxMenuItem( m_gizmosMenu, wxID_ANY, wxString( _("Physics") ) , wxEmptyString, wxITEM_CHECK );
	m_gizmosMenu->Append( m_menuItemPhysics );

	wxMenuItem* m_menuItemObject;
	m_menuItemObject = new wxMenuItem( m_gizmosMenu, wxID_ANY, wxString( _("Objects") ) , wxEmptyString, wxITEM_CHECK );
	m_gizmosMenu->Append( m_menuItemObject );

	m_menubar1->Append( m_gizmosMenu, _("Gizmos") );

	this->SetMenuBar( m_menubar1 );

	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( MainWindow::OnExit ) );
	this->Connect( wxEVT_MAXIMIZE, wxMaximizeEventHandler( MainWindow::OnMaximize ) );
	m_treeNode->Connect( wxEVT_COMMAND_TREE_BEGIN_DRAG, wxTreeEventHandler( MainWindow::OnStartDrag ), NULL, this );
	m_treeNode->Connect( wxEVT_COMMAND_TREE_END_DRAG, wxTreeEventHandler( MainWindow::OnEndDrag ), NULL, this );
	m_treeNode->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( MainWindow::OnTreeSelChanged ), NULL, this );
	m_Play->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnPlay ), NULL, this );
	m_Pause->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnPause ), NULL, this );
	m_Stop->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnStop ), NULL, this );
	m_cameraSelector->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( MainWindow::OnCameraSelected ), NULL, this );
	m_WT->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnModificatorClick ), NULL, this );
	m_WR->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnModificatorClick ), NULL, this );
	m_LT->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnModificatorClick ), NULL, this );
	m_LR->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnModificatorClick ), NULL, this );
	m_Snap->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainWindow::OnSnap ), NULL, this );
	m_renderPanel->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( MainWindow::OnMouseClick ), NULL, this );
	m_renderPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( MainWindow::OnMouseDown ), NULL, this );
	m_renderPanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( MainWindow::OnMouseUp ), NULL, this );
	m_renderPanel->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( MainWindow::OnMouseDown ), NULL, this );
	m_renderPanel->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( MainWindow::OnMouseUp ), NULL, this );
	m_renderPanel->Connect( wxEVT_MOTION, wxMouseEventHandler( MainWindow::OnMotion ), NULL, this );
	m_renderPanel->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MainWindow::OnMouseWheel ), NULL, this );
	m_renderPanel->Connect( wxEVT_PAINT, wxPaintEventHandler( MainWindow::OnMyPaint ), NULL, this );
	m_renderPanel->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainWindow::OnMouseDown ), NULL, this );
	m_renderPanel->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( MainWindow::OnMouseUp ), NULL, this );
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnNew ), this, m_menuIItemNew->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnOpen ), this, m_menuItemOpen->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnSaveAs ), this, m_menuItemSaveAs->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnSave ), this, m_menuItemSave->GetId());
	m_menu1->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnExit ), this, m_menuItemExit->GetId());
	m_gizmosMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnGizmosGrid ), this, m_menuItemGrid->GetId());
	m_gizmosMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnGizmosText ), this, m_menuItemDebugText->GetId());
	m_gizmosMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnGizmosPhysics ), this, m_menuItemPhysics->GetId());
	m_gizmosMenu->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainWindow::OnGizmosObjects ), this, m_menuItemObject->GetId());
}

MainWindow::~MainWindow()
{
}
