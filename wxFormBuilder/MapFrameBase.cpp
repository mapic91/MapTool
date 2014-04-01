///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "MapFrameBase.h"

///////////////////////////////////////////////////////////////////////////

MapFrameBase::MapFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_OpenMap = new wxButton( m_toolBar1, wxID_ANY, wxT("打开地图..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_OpenMap );
	m_SaveToPng = new wxButton( m_toolBar1, wxID_ANY, wxT("存为PNG..."), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_SaveToPng );
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_Layer1 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer1->SetValue(true); 
	m_toolBar1->AddControl( m_Layer1 );
	m_Layer2 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer2->SetValue(true); 
	m_toolBar1->AddControl( m_Layer2 );
	m_Layer3 = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer3->SetValue(true); 
	m_toolBar1->AddControl( m_Layer3 );
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_Trap = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("陷阱"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Trap );
	m_Barrer = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("障碍"), wxDefaultPosition, wxDefaultSize, 0 );
	m_toolBar1->AddControl( m_Barrer );
	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->AddSeparator(); 
	
	m_LayerTransparent = new wxCheckBox( m_toolBar1, wxID_ANY, wxT("图层透明"), wxDefaultPosition, wxDefaultSize, 0 );
	m_LayerTransparent->SetToolTip( wxT("使图层透明的地方透明，而不是黑色") );
	
	m_toolBar1->AddControl( m_LayerTransparent );
	m_toolBar1->Realize(); 
	
	bSizer2->Add( m_toolBar1, 1, wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_MapView = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3->Add( m_MapView, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_StatusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	m_menubar3 = new wxMenuBar( 0 );
	m_File = new wxMenu();
	wxMenuItem* m_menuItemUP;
	m_menuItemUP = new wxMenuItem( m_File, ID_MAPUP, wxString( wxT("地图上移\tUp") ) , wxEmptyString, wxITEM_NORMAL );
	m_File->Append( m_menuItemUP );
	
	wxMenuItem* m_menuItemDOWN;
	m_menuItemDOWN = new wxMenuItem( m_File, ID_MAPDOWN, wxString( wxT("地图下移\tDown") ) , wxEmptyString, wxITEM_NORMAL );
	m_File->Append( m_menuItemDOWN );
	
	wxMenuItem* m_menuItemLeft;
	m_menuItemLeft = new wxMenuItem( m_File, ID_MAPLEFT, wxString( wxT("地图左移\tLeft") ) , wxEmptyString, wxITEM_NORMAL );
	m_File->Append( m_menuItemLeft );
	
	wxMenuItem* m_menuItemRIGHT;
	m_menuItemRIGHT = new wxMenuItem( m_File, ID_MAPRIGHT, wxString( wxT("地图右移\tRight") ) , wxEmptyString, wxITEM_NORMAL );
	m_File->Append( m_menuItemRIGHT );
	
	m_menubar3->Append( m_File, wxT("地图") ); 
	
	this->SetMenuBar( m_menubar3 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_OpenMap->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
	m_Layer1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
	m_Trap->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnTrap ), NULL, this );
	m_Barrer->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnBarrer ), NULL, this );
	m_LayerTransparent->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayerTransparent ), NULL, this );
	m_MapView->Connect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMouseMove ), NULL, this );
	m_MapView->Connect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnMapDraw ), NULL, this );
	this->Connect( m_menuItemUP->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapUp ) );
	this->Connect( m_menuItemDOWN->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapDown ) );
	this->Connect( m_menuItemLeft->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapLeft ) );
	this->Connect( m_menuItemRIGHT->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapRight ) );
}

MapFrameBase::~MapFrameBase()
{
	// Disconnect Events
	m_OpenMap->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
	m_Layer1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
	m_Trap->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnTrap ), NULL, this );
	m_Barrer->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnBarrer ), NULL, this );
	m_LayerTransparent->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayerTransparent ), NULL, this );
	m_MapView->Disconnect( wxEVT_MOTION, wxMouseEventHandler( MapFrameBase::OnMouseMove ), NULL, this );
	m_MapView->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MapFrameBase::OnMapDraw ), NULL, this );
	this->Disconnect( ID_MAPUP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapUp ) );
	this->Disconnect( ID_MAPDOWN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapDown ) );
	this->Disconnect( ID_MAPLEFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapLeft ) );
	this->Disconnect( ID_MAPRIGHT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MapFrameBase::OnMapRight ) );
	
}
