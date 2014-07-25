///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jun  5 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAPFRAMEBASE_H__
#define __MAPFRAMEBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
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
#include <wx/combobox.h>
#include <wx/bmpcbox.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

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
#define ID_MAPUP 1009
#define ID_MAPDOWN 1010
#define ID_MAPLEFT 1011
#define ID_MAPRIGHT 1012
#define ID_DIRECTION 1013

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
		wxSplitterWindow* m_splitter2;
		wxPanel* m_MapView;
		wxPanel* m_panel10;
		wxSplitterWindow* m_splitter4;
		wxPanel* m_MapControl;
		wxPanel* m_panel12;
		wxNotebook* m_notebook1;
		wxPanel* m_panel5;
		wxListCtrl* m_NpcList;
		wxPanel* m_panel6;
		wxListCtrl* m_ObjList;
		wxStatusBar* m_StatusBar;
		wxMenuBar* m_menubar3;
		wxMenu* m_MenuFile;
		wxMenu* m_MenuCharacter;
		wxMenu* m_menu3;
		
		// Virtual event handlers, overide them in your derived class
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
		virtual void OnMapCtrlMouseMotion( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnDrawMapControl( wxPaintEvent& event ) { event.Skip(); }
		virtual void SaveToPNG( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapUp( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapDown( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapLeft( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMapRight( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadCharater( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCharacterDirection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportNpcFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputNpcFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowNpcCounts( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearNpc( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoadObject( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnObjectDirection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImportObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOutputObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnShowObjCount( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearObj( wxCommandEvent& event ) { event.Skip(); }
		
	
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
		wxComboBox* m_ShowName;
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
		wxButton* m_button26;
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
		virtual void OnSaveNpcIniFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancle( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NpcItemEditDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,600 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~NpcItemEditDialogBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ObjItemEditDialogBase
///////////////////////////////////////////////////////////////////////////////
class ObjItemEditDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText40;
		wxComboBox* m_ObjName;
		wxStaticText* m_staticText41;
		wxComboBox* m_Kind;
		wxStaticText* m_staticText42;
		wxComboBox* m_Dir;
		wxStaticText* m_staticText43;
		wxComboBox* m_Damage;
		wxStaticText* m_staticText44;
		wxComboBox* m_Frame;
		wxStaticText* m_staticText45;
		wxComboBox* m_Height;
		wxStaticText* m_staticText52;
		wxComboBox* m_Lum;
		wxStaticText* m_staticText46;
		wxButton* m_ObjFile;
		wxButton* m_button22;
		wxStaticText* m_staticText47;
		wxButton* m_ScriptFile;
		wxButton* m_button23;
		wxStaticText* m_staticText48;
		wxButton* m_WavFile;
		wxButton* m_button24;
		wxStaticText* m_staticText49;
		wxComboBox* m_comboBox41;
		wxStaticText* m_staticText50;
		wxComboBox* m_OffX;
		wxStaticText* m_staticText51;
		wxComboBox* m_OffY;
		wxButton* m_button25;
		wxButton* m_OK;
		wxButton* m_Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEditObjFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnScriptFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearScriptFile( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnEditScriptFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnWavFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearWavFile( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnOpenWavFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSaveObjIniFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOk( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancle( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ObjItemEditDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 620,252 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ObjItemEditDialogBase();
	
};

#endif //__MAPFRAMEBASE_H__
