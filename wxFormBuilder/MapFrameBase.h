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
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MapFrameBase
///////////////////////////////////////////////////////////////////////////////
class MapFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxButton* m_OpenMap;
		wxButton* m_SaveToPng;
		wxCheckBox* m_Layer1;
		wxCheckBox* m_Layer2;
		wxCheckBox* m_Layer3;
		wxStaticLine* m_staticline1;
		wxCheckBox* m_Trap;
		wxCheckBox* m_Barrer;
		wxPanel* m_MapView;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OpenMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer3( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTrap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBarrer( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MapFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 632,352 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MapFrameBase();
	
};

#endif //__MAPFRAMEBASE_H__
