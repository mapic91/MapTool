///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAPFRAMEBASE_H__
#define __MAPFRAMEBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/toolbar.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_MAPUP 1000
#define ID_MAPDOWN 1001
#define ID_MAPLEFT 1002
#define ID_MAPRIGHT 1003

///////////////////////////////////////////////////////////////////////////////
/// Class MapFrameBase
///////////////////////////////////////////////////////////////////////////////
class MapFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxButton* m_OpenMap;
		wxButton* m_SaveToPng;
		wxCheckBox* m_Layer1;
		wxCheckBox* m_Layer2;
		wxCheckBox* m_Layer3;
		wxCheckBox* m_Trap;
		wxCheckBox* m_Barrer;
		wxCheckBox* m_LayerTransparent;
		wxPanel* m_MapView;
		wxStatusBar* m_StatusBar;
		wxMenuBar* m_menubar3;
		wxMenu* m_File;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OpenMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer3( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTrap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBarrer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerTransparent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapDraw( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMapUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapRight( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MapFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 632,352 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MapFrameBase();
	
};

#endif //__MAPFRAMEBASE_H__
