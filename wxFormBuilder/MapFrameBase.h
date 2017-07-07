///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAPFRAMEBASE_H__
#define __MAPFRAMEBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class wxListView;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/propgrid/advprops.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>

///////////////////////////////////////////////////////////////////////////

#define ID_OPENMAP 1000
#define ID_NPCMODE 1001
#define ID_OBJMODE 1002
#define ID_TOOLPLACE 1003
#define ID_TOOLDELETE 1004
#define ID_EDITATTRIBUTE 1005
#define ID_TOOLMOVE 1006
#define ID_SHOWNPC 1007
#define ID_SHOWOBJ 1008
#define MYID_NPCLISTCTRL 1009
#define MYID_OBJLISTCTRL 1010
#define ID_MAPUP 1011
#define ID_MAPDOWN 1012
#define ID_MAPLEFT 1013
#define ID_MAPRIGHT 1014
#define ID_DIRECTION 1015
#define MYID_MAPVIEW_COPY 1016
#define MYID_MAPVIEW_CUT 1017
#define MYID_MAPVIEW_PASTE 1018
#define MYID_MAPVIEW_DETIAL 1019
#define MYID_MAPVIEW_DELETE 1020
#define MYID_MAPVIEW_BATEDIT 1021
#define MYID_MAPVIEW_CLEAR_SELECTION 1022

///////////////////////////////////////////////////////////////////////////////
/// Class MapFrameBase
///////////////////////////////////////////////////////////////////////////////
class MapFrameBase : public wxFrame 
{
	private:
	
	protected:
		wxToolBar* m_toolBar1;
		wxToolBarToolBase* m_tool7; 
		wxCheckBox* m_Layer1;
		wxCheckBox* m_Layer2;
		wxCheckBox* m_Layer3;
		wxCheckBox* m_Trap;
		wxCheckBox* m_Barrer;
		wxCheckBox* m_LayerTransparent;
		wxToolBar* m_ToolBarEdit;
		wxToolBarToolBase* m_tool5; 
		wxToolBarToolBase* m_tool6; 
		wxToolBarToolBase* m_ToolPlace; 
		wxToolBarToolBase* m_ToolDelete; 
		wxToolBarToolBase* m_ToolEditAttribute; 
		wxToolBarToolBase* m_tool4; 
		wxToolBarToolBase* m_tool8; 
		wxToolBarToolBase* m_tool9; 
		wxSplitterWindow* m_mapViewWindow;
		wxPanel* m_MapView;
		wxPanel* m_SideCtrl;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_MapControl;
		wxPanel* m_panelList;
		wxNotebook* m_ListData;
		wxPanel* m_panel5;
		wxListView* m_npcListCtrl;
		wxPanel* m_panel6;
		wxListView* m_objListCtrl;
		wxStatusBar* m_StatusBar;
		wxMenuBar* m_menubar3;
		wxMenu* m_MenuFile;
		wxMenu* m_menuEdit;
		wxMenu* m_MenuCharacter;
		wxMenu* m_menu3;
		wxMenu* m_menu5;
		wxMenu* m_menu6;
		wxMenu* m_menuMapView;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OpenMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer3( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTrap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBarrer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerTransparent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNpcMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnObjMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlaceMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditAttributeMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowNpcCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowObjCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapViewMouseLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMapViewMouseLeftUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMapDraw( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMapViewMouseRightUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMapCtrlMouseMotion( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnDrawMapControl( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnListCtrlLeftDClick( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnListItemFocused( wxListEvent& event ) { event.Skip(); }
		virtual void OnListCtrlRightDown( wxListEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUndo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRedo( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadCharater( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCharacterDirection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportNpcFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputNpcFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRepositionUnseenNpc( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowNpcCounts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearNpc( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadObject( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnObjectDirection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRepositionUnseenObj( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowObjCount( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearObj( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetFps( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetTmxHelperPort( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapViewMenu( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MapFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 632,352 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MapFrameBase();
		
		void m_mapViewWindowOnIdle( wxIdleEvent& )
		{
			m_mapViewWindow->SetSashPosition( -200 );
			m_mapViewWindow->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_mapViewWindowOnIdle ), NULL, this );
		}
		
		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( 100 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_splitter4OnIdle ), NULL, this );
		}
		
		void MapFrameBaseOnContextMenu( wxMouseEvent &event )
		{
			this->PopupMenu( m_menuMapView, event.GetPosition() );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NpcItemEditDialogBase
///////////////////////////////////////////////////////////////////////////////
class NpcItemEditDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText54;
		wxPropertyGridManager* m_GridManager;
		wxPropertyGridPage* m_PropertyPage;
		wxButton* m_button26;
		wxButton* m_OK;
		wxButton* m_Cancle;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSaveNpcIniFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancle( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NpcItemEditDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 620,714 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~NpcItemEditDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ObjItemEditDialogBase
///////////////////////////////////////////////////////////////////////////////
class ObjItemEditDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxPropertyGridManager* m_GridManager;
		wxPropertyGridPage* m_propertyGridPage2;
		wxButton* m_button25;
		wxButton* m_OK;
		wxButton* m_Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnSaveObjIniFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancle( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ObjItemEditDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 620,447 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ObjItemEditDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SetTmxHelperPortDialogBase
///////////////////////////////////////////////////////////////////////////////
class SetTmxHelperPortDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText53;
		wxSpinCtrl* m_spinCtrlPort;
		wxButton* m_buttonOK;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonOKClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SetTmxHelperPortDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("SetPort"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 279,104 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~SetTmxHelperPortDialogBase();
	
};

#endif //__MAPFRAMEBASE_H__
