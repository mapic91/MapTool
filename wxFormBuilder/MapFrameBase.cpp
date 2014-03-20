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
	
	m_OpenMap = new wxButton( this, wxID_ANY, wxT("打开地图..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_OpenMap, 0, wxALL, 5 );
	
	m_SaveToPng = new wxButton( this, wxID_ANY, wxT("存为PNG..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_SaveToPng, 0, wxALL, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_scrolledWindow1 = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_scrolledWindow1->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_MapView = new wxStaticBitmap( m_scrolledWindow1, wxID_ANY, wxBitmap( wxT("../../../temp/New folder/480_800/01.jpg"), wxBITMAP_TYPE_ANY ), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_MapView, 1, wxALL, 5 );
	
	
	m_scrolledWindow1->SetSizer( bSizer4 );
	m_scrolledWindow1->Layout();
	bSizer4->Fit( m_scrolledWindow1 );
	bSizer3->Add( m_scrolledWindow1, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_OpenMap->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
}

MapFrameBase::~MapFrameBase()
{
	// Disconnect Events
	m_OpenMap->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
	
}
