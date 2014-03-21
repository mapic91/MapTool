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
	
	m_OpenMap = new wxButton( this, wxID_ANY, wxT("´ò¿ªµØÍ¼..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_OpenMap, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_SaveToPng = new wxButton( this, wxID_ANY, wxT("´æÎªPNG..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_SaveToPng, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Layer1 = new wxCheckBox( this, wxID_ANY, wxT("Í¼²ã1"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer1->SetValue(true); 
	bSizer2->Add( m_Layer1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Layer2 = new wxCheckBox( this, wxID_ANY, wxT("Í¼²ã2"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer2->SetValue(true); 
	bSizer2->Add( m_Layer2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_Layer3 = new wxCheckBox( this, wxID_ANY, wxT("Í¼²ã3"), wxDefaultPosition, wxDefaultSize, 0 );
	m_Layer3->SetValue(true); 
	bSizer2->Add( m_Layer3, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_WindowMapView = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	m_WindowMapView->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_MapView = new wxStaticBitmap( m_WindowMapView, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_MapView, 1, wxALL, 5 );
	
	
	m_WindowMapView->SetSizer( bSizer4 );
	m_WindowMapView->Layout();
	bSizer4->Fit( m_WindowMapView );
	bSizer3->Add( m_WindowMapView, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_OpenMap->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
	m_Layer1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
}

MapFrameBase::~MapFrameBase()
{
	// Disconnect Events
	m_OpenMap->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::OpenMap ), NULL, this );
	m_SaveToPng->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MapFrameBase::SaveToPNG ), NULL, this );
	m_Layer1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer1 ), NULL, this );
	m_Layer2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer2 ), NULL, this );
	m_Layer3->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MapFrameBase::OnLayer3 ), NULL, this );
	
}
