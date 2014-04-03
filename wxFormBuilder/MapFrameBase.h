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
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/menu.h>
#include <wx/frame.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/bmpcbox.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define ID_TOOLPLACE 1000
#define ID_TOOLDELETE 1001
#define ID_EDITATTRIBUTE 1002
#define ID_TOOLMOVE 1003
#define ID_MAPUP 1004
#define ID_MAPDOWN 1005
#define ID_MAPLEFT 1006
#define ID_MAPRIGHT 1007
#define ID_DIRECTION 1008

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
		wxToolBar* m_ToolBarEdit;
		wxSplitterWindow* m_splitter2;
		wxPanel* m_MapView;
		wxPanel* m_panel10;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_MapControl;
		wxPanel* m_panel12;
		wxStatusBar* m_StatusBar;
		wxMenuBar* m_menubar3;
		wxMenu* m_MenuFile;
		wxMenu* m_MenuCharacter;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OpenMap( wxCommandEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayer3( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTrap( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBarrer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLayerTransparent( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlaceMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeleteMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditAttributeMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMoveMode( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapViewMouseLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMouseMove( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMapDraw( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMapCtrlMouseMotion( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnDrawMapControl( wxPaintEvent& event ) { event.Skip(); }
		virtual void OnMapUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadCharater( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCharacterDirection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputNpcFile( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MapFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 632,352 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MapFrameBase();
		
		void m_splitter2OnIdle( wxIdleEvent& )
		{
			m_splitter2->SetSashPosition( -200 );
			m_splitter2->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_splitter2OnIdle ), NULL, this );
		}
		
		void m_splitter4OnIdle( wxIdleEvent& )
		{
			m_splitter4->SetSashPosition( 100 );
			m_splitter4->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MapFrameBase::m_splitter4OnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class NpcItemEditDialogBase
///////////////////////////////////////////////////////////////////////////////
class NpcItemEditDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxComboBox* m_Name;
		wxStaticText* m_staticText2;
		wxComboBox* m_Evade;
		wxStaticText* m_staticText3;
		wxComboBox* m_Kind;
		wxStaticText* m_staticText4;
		wxComboBox* m_Attack;
		wxStaticText* m_staticText5;
		wxComboBox* m_Relation;
		wxStaticText* m_staticText6;
		wxComboBox* m_AttackLevel;
		wxStaticText* m_staticText7;
		wxComboBox* m_PathFinder;
		wxStaticText* m_staticText8;
		wxComboBox* m_Defend;
		wxStaticText* m_staticText9;
		wxComboBox* m_State;
		wxStaticText* m_staticText10;
		wxComboBox* m_comboBox10;
		wxStaticText* m_staticText11;
		wxComboBox* m_comboBox11;
		wxStaticText* m_staticText12;
		wxBitmapComboBox* m_Exp;
		wxStaticText* m_staticText13;
		wxComboBox* m_VisionRadius;
		wxStaticText* m_staticText14;
		wxComboBox* m_LevelUpExp;
		wxStaticText* m_staticText15;
		wxComboBox* m_DialogRadius;
		wxStaticText* m_staticText16;
		wxComboBox* m_Level;
		wxStaticText* m_staticText17;
		wxComboBox* m_AttackRadius;
		wxStaticText* m_staticText18;
		wxComboBox* m_comboBox17;
		wxStaticText* m_staticText19;
		wxComboBox* m_Dir;
		wxStaticText* m_staticText20;
		wxComboBox* m_Life;
		wxStaticText* m_staticText21;
		wxComboBox* m_Lum;
		wxStaticText* m_staticText22;
		wxComboBox* m_LifeMax;
		wxStaticText* m_staticText23;
		wxComboBox* m_Action;
		wxStaticText* m_staticText24;
		wxComboBox* m_Thew;
		wxStaticText* m_staticText25;
		wxComboBox* m_WalkSpeed;
		wxStaticText* m_staticText26;
		wxComboBox* m_ThewMax;
		wxStaticText* m_staticText27;
		wxComboBox* m_comboBox26;
		wxStaticText* m_staticText28;
		wxComboBox* m_Mana;
		wxStaticText* m_staticText29;
		wxButton* m_NpcIni;
		wxButton* m_NpcIniEdit;
		wxStaticText* m_staticText30;
		wxComboBox* m_ManaMax;
		wxStaticText* m_staticText31;
		wxButton* m_BodyIni;
		wxButton* m_BodyIniEdit;
		wxStaticText* m_staticText32;
		wxComboBox* m_comboBox31;
		wxStaticText* m_staticText33;
		wxButton* m_FlyIni;
		wxButton* m_FlyIniEdit;
		wxStaticText* m_staticText34;
		wxComboBox* m_ExpBonus;
		wxStaticText* m_staticText35;
		wxButton* m_FlyIni2;
		wxButton* m_FlyIni2Edit;
		wxStaticText* m_staticText36;
		wxComboBox* m_Idle;
		wxStaticText* m_staticText41;
		wxButton* m_ScriptFile;
		wxButton* m_ScriptFileEdit;
		wxStaticText* m_staticText42;
		wxTextCtrl* m_FixedPos;
		wxStaticText* m_staticText43;
		wxButton* m_DeathScript;
		wxButton* m_DeathScriptEdit;
		wxButton* m_OK;
		wxButton* m_Cancle;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNpcIni( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNpcIniEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBodyIni( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBodyIniClear( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnBodyIniEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFlyIni( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFlyIniClear( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnFlyIniEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFlyIni2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFlyIni2Clear( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnFlyIni2Edit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnScriptFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnScriptFileClear( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnScriptFileEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeathScript( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeathScriptClear( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnDeathScriptEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancle( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NpcItemEditDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,600 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~NpcItemEditDialogBase();
	
};

#endif //__MAPFRAMEBASE_H__
