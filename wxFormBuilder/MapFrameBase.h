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
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/scrolwin.h>
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
		wxScrolledWindow* m_scrolledWindow1;
		wxStaticBitmap* m_MapView;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OpenMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MapFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MapFrameBase();
	
};

#endif //__MAPFRAMEBASE_H__
