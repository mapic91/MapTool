#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>
#include "Map.h"
#include "Npc.hpp"
#include "AsfDecode.hpp"
#include "ClipBoard.h"
#include "TmxReaderSetting.h"

#include "wx/dcbuffer.h"
#include "wx/filedlg.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"
#include "wx/timer.h"
#include "wx/cmdproc.h"
#include "MapToolCommand.h"

#include <map>
#include <vector>
#include <memory>

class NpcItemEditDialog;
struct LevelDetial;
extern std::map<int, LevelDetial> *g_NpcLevelList;

struct LevelDetial
{
    int Life;
    int Attack;
    int Defend;
    int Evade;
};

std::vector<long> GetAllSelectedItems(const wxListView *listCtrl);
void DeselectAllItem(wxListView *listCtrl);

class MapTool : public MapFrameBase
{
public:
    MapTool(wxWindow* parent);
    virtual ~MapTool();

public:
	void RedrawMapView();
	void RedrawNow();
	//Disable timer to prevent from message quene full of timer event, use EnableTimer to enable.
	void DisableTimer();
	void EnableTimer();

public:
	void ReNewNpcAsf(NpcItem *item);
	void ReNewObjAsf(ObjItem *item);

protected:
public:

    enum MYID
    {
        ID_SHOWNPCKEY = 10000,
        ID_SHOWOBJKEY,
        ID_ATTRIBUTELISTITEM,
        ID_DELETELISTITEM,
        ID_BATEDITLISTITEM,
        ID_SETFIXPOSUNDO,
        ID_SETFIXPOSCOMPLETE,
    };

	void Exit();
    void FrameOnChar( wxKeyEvent& event );
    void OnClose( wxCloseEvent& event );
    void OpenMap(wxCommandEvent& event);
    void SaveToPNG(wxCommandEvent& event);
    void OnLayer1( wxCommandEvent& event ) ;
    void OnLayer2( wxCommandEvent& event );
    void OnLayer3( wxCommandEvent& event );
    void OnTrap( wxCommandEvent& event ) ;
    void OnBarrer( wxCommandEvent& event ) ;
    void OnMapDraw( wxPaintEvent& event ) ;
    void OnLayerTransparent( wxCommandEvent& event );

    void OnMapUp( wxCommandEvent& event ) ;
    void OnMapDown( wxCommandEvent& event ) ;
    void OnMapLeft( wxCommandEvent& event ) ;
    void OnMapRight( wxCommandEvent& event ) ;
    void OnMapViewMouseLeftDown( wxMouseEvent& event );
    void OnMapViewMouseLeftUp( wxMouseEvent& event );
    void OnMapViewMouseRightUp( wxMouseEvent& event );

    void OnMouseMove( wxMouseEvent& event );

    void OnDrawMapControl( wxPaintEvent& event );
    void OnMapCtrlMouseMotion( wxMouseEvent& event );

    void OnNpcMode( wxCommandEvent& event )
    {
        m_isNpc = true;
        m_isObj = false;
        m_ToolBarEdit->ToggleTool(ID_NPCMODE, true);
        m_ToolBarEdit->ToggleTool(ID_OBJMODE, false);
        RedrawMapView();
    }
    void OnObjMode( wxCommandEvent& event )
    {
        m_isNpc = false;
        m_isObj = true;
        m_ToolBarEdit->ToggleTool(ID_NPCMODE, false);
        m_ToolBarEdit->ToggleTool(ID_OBJMODE, true);
        RedrawMapView();
    }

    //Edit
	void OnUndo( wxCommandEvent& event );
	void OnRedo( wxCommandEvent& event );

	//Npc obj edit dialog
    void ShowNpcItemEditor(long npcitemidx);
    void ShowNpcItemEditor(NpcItem *item, long index = -1);
    /* batch edit, items count must greater than 1 */
    void ShowNpcItemEditor(const std::vector<long> &items);
    void ShowObjItemEditor(long objitemidx);
    void ShowObjItemEditor(ObjItem *item, long index = -1);
    /* batch edit, items count must greater than 1 */
    void ShowObjItemEditor(const std::vector<long> &items);
    void NpcItemEditShowModle(NpcItemEditDialog *dialog, NpcItem *npcitem);

	//Map view
	virtual void OnMapViewMenu( wxCommandEvent& event );
	virtual void OnMapViewCopy( wxCommandEvent& event );
	virtual void OnMapViewCut( wxCommandEvent& event );
	virtual void OnMapViewPaste( wxCommandEvent& event );
	virtual void OnMapViewDetail( wxCommandEvent& event );
	virtual void OnMapViewDelete( wxCommandEvent& event );
	virtual void OnMapViewBatEdit( wxCommandEvent& event);
	virtual void OnMapViewClearSelection( wxCommandEvent& event);
	/*isMulty - is in multy copy/cut mode(CTRL button pressed)*/
	void SetMapViewPopupMenuState(bool hasItem, bool canPaste, bool isMulty);
	void PopupMapViewMenu();
    /** \brief Convert map tile position to screen position.
     *
     * \param tileX Map tile position x
     * \param tileY Map tile position y
	 * \param poxX[out] Position x
     * \return poxY[out] Position y
     *
     */
	void MapPositionToScreenPositon(int tileX, int tileY, int *posX, int *posY);
	bool IsItemSelectedAtTile(int tileX, int tileY);

	//Timer
	void OnTimer(wxTimerEvent &event);
	virtual void OnSetFps( wxCommandEvent& event );

	//Tmx
	virtual void OnSetTmxHelperPort( wxCommandEvent& event );

    //NPC
    void OnLoadCharater( wxCommandEvent& event );
    void OnPlaceMode( wxCommandEvent& event ) ;
    void OnCharacterDirection( wxCommandEvent& event );
    void OnImportNpcFile( wxCommandEvent& event );
    void OnOutputNpcFile( wxCommandEvent& event );
    void OnRepositionUnseenNpc( wxCommandEvent& event );
    void OnShowNpcCounts( wxCommandEvent& event )
    {
        wxMessageBox(wxString::Format(wxT("NPC 数量：Count = %d"), m_NpcList.getCounts()),
                     wxT("消息"));
    }
    void OnClearNpc( wxCommandEvent& event )
    {
    	if(wxMessageBox(wxT("清除所有NPC？"),
						wxMessageBoxCaptionStr,
						wxYES_NO | wxCENTER | wxICON_QUESTION) == wxYES)
		{
			MTC_Delete_Npcs *cmd = new MTC_Delete_Npcs(wxT("清除所有NPC"));
			for(auto item : m_NpcList)
			{
				cmd->DeleteItem(0, item);
			}
			m_commandProcessor.Store(cmd);

			m_NpcList.Clear();
			RefreshNpcList();
		}
    }

    //OBJ
    void OnLoadObject( wxCommandEvent& event );
    void OnObjectDirection( wxCommandEvent& event )
    {
        OnCharacterDirection(event);
    }
    void OnImportObjFile( wxCommandEvent& event );
    void OnOutputObjFile( wxCommandEvent& event );
    void OnRepositionUnseenObj( wxCommandEvent& event );
    void OnShowObjCount( wxCommandEvent& event )
    {
        wxMessageBox(wxString::Format(wxT("OBJ 数量：Count = %d"), m_ObjList.getCounts()),
                     wxT("消息"));
    }
    void OnClearObj( wxCommandEvent& event )
    {
    	if(wxMessageBox(wxT("清除所有OBJ？"),
						wxMessageBoxCaptionStr,
						wxYES_NO | wxCENTER | wxICON_QUESTION) == wxYES)
		{
			MTC_Delete_Objs *cmd = new MTC_Delete_Objs(wxT("清除所有OBJ"));
			for(auto item : m_ObjList)
			{
				cmd->DeleteItem(0, item);
			}
			m_commandProcessor.Store(cmd);

			m_ObjList.Clear();
			RefreshObjList();
		}
    }

    void OnDeleteMode( wxCommandEvent& event );

    void OnMoveMode( wxCommandEvent& event );

    void OnEditAttributeMode( wxCommandEvent& event );

    void OnShowNpcCheck( wxCommandEvent& event )
    {
        RedrawMapView();
    }
    void OnShowObjCheck( wxCommandEvent& event )
    {
        RedrawMapView();
    }
    void OnShowNpcCheckKey( wxCommandEvent& event)
    {
        m_ToolBarEdit->ToggleTool(ID_SHOWNPC, !m_ToolBarEdit->GetToolState(ID_SHOWNPC));
        RedrawMapView();
    }
    void OnShowObjCheckKey( wxCommandEvent& event)
    {
        m_ToolBarEdit->ToggleTool(ID_SHOWOBJ, !m_ToolBarEdit->GetToolState(ID_SHOWOBJ));
        RedrawMapView();
    }

    //ListData
    void OnListCtrlLeftDClick( wxMouseEvent& event );
    void OnListCtrlRightDown( wxListEvent& event );
    void OnListItemFocused( wxListEvent& event );
    void RefreshNpcList();
    void RefreshObjList();
    void ShowTile(int tileX, int tileY);
    void DeleteListItem( wxCommandEvent& event);
    void AttributeListItem( wxCommandEvent& event);
    void BatEditListItem( wxCommandEvent& event);
    void ShowAttributeListItem();
    void ShowBatEditDialog();
    void UpdateListItem(int index, int listType = NPCLIST);
    void UpdateListItem(NpcItem *item);
    void UpdateListItem(ObjItem *item);
    bool IsInSelectingItem(){return wxGetKeyState(WXK_CONTROL);}
    void ToggleNpcSelection(int tileX, int tileY);
    void ToggleObjSelection(int tileX, int tileY);
    void ShowNpcSelectionCountMessage();
    void ShowObjSelectionCountMessage();
    void DoBatchRectangleSelection();
    bool IsInMultyPaste();
    /** \brief Show tile in map view and show YesNoAllDialog at than position.
     *
     * \param tileX Tile x position
     * \param tileY Tile y position
     * \return YesNoAllDialog returned value
     *
     */
    int ShowYesNoAllInPosition(int tileX, int tileY, wxString message = wxEmptyString);
    /** \brief Draw position mark for selected item in list
     *
     */
    void DrawListSelection(wxDC &dc);
    int GetItemSelectionCount();
    enum  {NPCLIST, OBJLIST};
    int m_curList, m_curListIndex;
    bool m_placeModeNotDraw;

    //currentView: if true dc is the current mapview client,
    // else dc is the entire map
    void DrawRectangle(long col, long row, wxDC &dc, bool currentView = true);
    //Draw a rectangle which top-left at tileStart bottom-right at tileEnd
    //currentView: if true dc is the current mapview client,
    // else dc is the entire map
    void DrawRectangle(wxPoint tileStart, wxPoint tileEnd, wxDC &dc, bool currentView = true);
    void DrawTile(long col,
                  long row,
                  wxDC &dc,
                  NpcItem *npcitem,
                  ObjItem *objitem = NULL,
                  bool currentView = true);
    bool IsDrawObjsNpcs();
    void DrawObjsNpcs(wxDC &dc, bool currentView = true);
    void DrawObjsNpcsPosition(wxDC &dc, bool currentView = true);
    void DrawNpcPosition(wxDC &dc, bool currentView = true);
    void DrawObjPostion(wxDC &dc, bool currentView = true);

    //if getImg is true return an image, else return NULL
    wxImage* ReadMap(bool getImg = false);
    void CheckMapViewBeginPosition();

    bool PromptDelection();

    bool ToTilePositionFromWindowPositon(int windowX, int windowY, int *tileX, int *tileY);

    wxBitmap m_MapBitmap;
    Map map;
    int m_ViewBeginx, m_ViewBeginy;
    //Current tile under mouse
    int m_CurTileX, m_CurTileY;
    wxString exepath, m_MapFileName;
    char m_NpcCurrentDir;
    bool m_isPlaceMode, m_isDeleteMode, m_isEditAttribute, m_isMoveMode;
    NpcItemEditDialog *m_npcItemEdit;

    //Npc obj list
    bool AddItem(NpcItem *item);
    bool AddItem(ObjItem *item);
    void DeleteNpcItem(int index);
    void DeleteObjItem(int index);
    NpcItem m_PlaceNpcData, // Npc to place
			*m_MoveNpcItem, // Npc in move
			*m_selectedNpcItemByRightUp; //Selected npc on right click in map view
    ObjItem m_PlaceObjData, // Obj to place
			*m_MoveObjItem, // Obj in move
			*m_selectedObjItemByRightUp; // Selected obj on right click in map view
	ClipBoard m_clipBoard;
	bool m_popupMenuShowed;
    NpcList m_NpcList;
    ObjList m_ObjList;
    AsfImgList *m_NpcAsfImgList, *m_ObjAsfImgList;
    wxString m_NpcIniFilePath, m_ObjIniFilePath;
    bool m_isObj, //Is in obj edit mode.
		 m_isNpc; //Is in npc edit mode.

    //File dialog path
    wxString m_NpcObjPath;
    wxString m_LastNpcListFileName;
    wxString m_LastObjListFileName;

    //FixPos
    void StartFixPosEdit(NpcItem *npcitem);
    void EndFixPosEdit(bool isCancle = false);
    void DrawEditFixPos(wxDC& dc);
    void CorrectFixedPos(NpcItem *item);
    bool m_isEditFixPos;
    NpcItem *m_fixPosEditItem;
    std::list<wxPoint> m_fixPosPoints;
    typedef std::list<wxPoint>::iterator FixPosListIterator;

    //Ctrl button pressed selection
    static wxPoint INVALID_TILE_POSITION;
    wxPoint m_batchSelectionBeginTile;
    wxPoint m_batchSelectionEndTile;

    //Batch copy/cut
    bool m_isBatch;
    NpcList m_npcsToPasted;
    ObjList m_objsToPasted;
    wxPoint m_tilePositionOffsetNpc;
    wxPoint m_tilePositionOffsetObj;
    static const wxKeyCode SELECTION_ADD_KEY = WXK_CONTROL_A;
    static const wxKeyCode SELECTION_SUB_KEY = WXK_CONTROL_S;

    wxCommandProcessor m_commandProcessor;
    std::unique_ptr<MTC_Move_Npc> mMoveCmdNpc;
    std::unique_ptr<MTC_Move_Obj> mMoveCmdObj;

    wxTimer m_timer;

    DECLARE_EVENT_TABLE()
};

class NpcItemEditDialog: public NpcItemEditDialogBase
{
public:
    NpcItemEditDialog(wxWindow *parent,
                      const wxString mapname,
                      AsfImgList *list,
                      NpcItem *item)
        :NpcItemEditDialogBase(parent)
    {
        m_mapName = mapname;
        m_NpcAsfImgList = list;
        m_item = item;

        exepath = wxStandardPaths::Get().GetExecutablePath();
        exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

        INI_MASK = wxT("INI文件(*.ini)|*.ini");
        TXT_MASK = wxT("TXT文件(*.txt)|*.txt");
        INI_MESSGEG = wxT("选择一个INI文件");
        STYLE = wxFD_OPEN;
    }
    virtual ~NpcItemEditDialog() {}

    void EnableFixpos(bool enable = true)
    {
    	m_FixedPos->Enable(enable);
    	m_FixedPosEdit->Enable(enable);
    }

    static wxString ToFixPosString(std::list<wxPoint> &list)
    {
        if(list.size() < 2) return wxEmptyString;
        wxString str;
        typedef std::list<wxPoint>::iterator Itor;
        int count = 0;
        int x, y;
        for(Itor it = list.begin(); it != list.end() && count < 8; it++, count++)
        {
            Map::GetTilePosition(&x, &y, it->x, it->y);
            str += PrintPosition(x);
            str += PrintPosition(y);
        }
        for(int i = 0; i < 8 - count; i++)
        {
            str += wxT("0000000000000000");
        }
        return str;
    }

    static wxString PrintPosition(int pos)
    {
        wxString tempstr = wxString::Format(wxT("%02X000000"),pos);
        return tempstr;
    }

    //Returned must deleted
    static std::list<wxPoint> *ToFixPosList(const wxString& str)
    {
        if(str.Length() != 128) return NULL;
        std::list<wxPoint> *list = new std::list<wxPoint>;
        wxString tempx, tempy;
        long x,y;
        int px, py;
        bool failed = false;
        for(int i = 0; i < 8; i++)
        {
            tempx = str.Mid(i * 16, 2);
            tempy = str.Mid(i * 16 + 8, 2);
            if(!tempx.ToLong(&x, 16) || !tempy.ToLong(&y, 16))
            {
                failed = true;
                break;
            }
            if(x == 0 || y == 0) break;
            Map::GetTileCenterPixelPosition(&px, &py, x, y);
            list->push_back(wxPoint((int)px,(int)py));
        }
        if(failed)
        {
            delete list;
            return NULL;
        }
        return list;
    }

    void InitFromNpcItem(NpcItem *item);
    /** \brief Assign to npc item from this dialog
     *
     * \param item Item to assign
     * \param onlySetted Only assign setted value in dialog to item if true.
     * \return
     *
     */

    void AssignToNpcItem(NpcItem *item, bool onlySetted = false);
    void SetFixPos(wxString str)
    {
        m_FixedPos->SetValue(str);
    }
    enum {OK, CANCEL, FIXPOSEDIT};
private:

    void OnOk( wxCommandEvent& event )
    {
        EndModal(OK);
    }
    void OnCancle( wxCommandEvent& event )
    {
        EndModal(CANCEL);
    }

    virtual void OnResetValue( wxMouseEvent& event )
    {
		switch(event.GetId())
		{
		case MYID_KIND:
			m_Kind->SetSelection(-1);
			break;
		case MYID_RELATION:
			m_Relation->SetSelection(-1);
			break;
		case MYID_DIR:
			m_Dir->SetSelection(-1);
			break;
		case MYID_ACTION:
			m_Action->SetSelection(-1);
			break;
		}
    }

    virtual void OnLevelChange( wxCommandEvent& event )
    {
        wxString value = event.GetString();
        long val;
        if(m_FillNpcLevelDetail->GetValue() &&
                value.ToLong(&val) &&
                g_NpcLevelList->count(val))
        {
            LevelDetial &detail = g_NpcLevelList->at(val);
            m_Evade->ChangeValue(wxString::Format(wxT("%d"), detail.Evade));
            m_Attack->ChangeValue(wxString::Format(wxT("%d"), detail.Attack));
            m_Defend->ChangeValue(wxString::Format(wxT("%d"), detail.Defend));
            m_Life->ChangeValue(wxString::Format(wxT("%d"), detail.Life));
            m_LifeMax->ChangeValue(wxString::Format(wxT("%d"), detail.Life));
        }
    }

    virtual void OnSetFixedPos( wxCommandEvent& event )
    {
        EndModal(FIXPOSEDIT);
    }

    void OnNpcIni( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             INI_MESSGEG,
                             exepath + wxT("ini\\npcres\\"),
                             wxT(""),
                             INI_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_NpcIni->SetLabel(filedlg.GetFilename());
            m_NpcIni->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnNpcIniEdit( wxCommandEvent& event )
    {
        wxString path = m_NpcIni->GetLabel();
        if(!path.IsEmpty()) path = wxT("ini\\npcres\\") + path;
        OpenFile(path);
    }
    void OnBodyIni( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             INI_MESSGEG,
                             exepath + wxT("ini\\obj\\"),
                             wxT(""),
                             INI_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_BodyIni->SetLabel(filedlg.GetFilename());
            m_BodyIni->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnBodyIniEdit( wxCommandEvent& event )
    {
        wxString path = m_BodyIni->GetLabel();
        if(!path.IsEmpty()) path = wxT("ini\\obj\\") + path;
        OpenFile(path);
    }
    void OnFlyIni( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             INI_MESSGEG,
                             exepath + wxT("ini\\magic\\"),
                             wxT(""),
                             INI_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_FlyIni->SetLabel(filedlg.GetFilename());
            m_FlyIni->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnFlyIniEdit( wxCommandEvent& event )
    {
        wxString path = m_FlyIni->GetLabel();
        if(!path.IsEmpty()) path = wxT("ini\\magic\\") + path;
        OpenFile(path);
    }
    void OnFlyIni2( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             INI_MESSGEG,
                             exepath + wxT("ini\\magic\\"),
                             wxT(""),
                             INI_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_FlyIni2->SetLabel(filedlg.GetFilename());
            m_FlyIni2->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnFlyIni2Edit( wxCommandEvent& event )
    {
        wxString path = m_FlyIni2->GetLabel();
        if(!path.IsEmpty()) path = wxT("ini\\magic\\") + path;
        OpenFile(path);
    }
    void OnScriptFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("选择一个脚本文件"),
                             exepath + wxT("script\\map\\") + m_mapName + wxT("\\"),
                             wxT(""),
                             TXT_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_ScriptFile->SetLabel(filedlg.GetFilename());
            m_ScriptFile->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnScriptFileEdit( wxCommandEvent& event )
    {
        wxString path = m_ScriptFile->GetLabel();
        if(!path.IsEmpty()) path = wxT("script\\map\\") + m_mapName + wxT("\\")+ path;
        OpenFile(path);
    }
    void OnDeathScript( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("选择一个脚本文件"),
                             exepath + wxT("script\\map\\") + m_mapName + wxT("\\"),
                             wxT(""),
                             TXT_MASK,
                             STYLE);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_DeathScript->SetLabel(filedlg.GetFilename());
            m_DeathScript->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnDeathScriptEdit( wxCommandEvent& event )
    {
        wxString path = m_DeathScript->GetLabel();
        if(!path.IsEmpty()) path = wxT("script\\map\\") + m_mapName + wxT("\\")+ path;
        OpenFile(path);
    }
	virtual void OnNpcIniClear( wxMouseEvent& event )
	{
		m_NpcIni->SetLabel(wxT(""));
		m_NpcIni->SetToolTip(wxT("左键选择，右键清除"));
	}
    void OnBodyIniClear( wxMouseEvent& event )
    {
        m_BodyIni->SetLabel(wxT(""));
        m_BodyIni->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnFlyIniClear( wxMouseEvent& event )
    {
        m_FlyIni->SetLabel(wxT(""));
        m_FlyIni->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnFlyIni2Clear( wxMouseEvent& event )
    {
        m_FlyIni2->SetLabel(wxT(""));
        m_FlyIni2->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnScriptFileClear( wxMouseEvent& event )
    {
        m_ScriptFile->SetLabel(wxT(""));
        m_ScriptFile->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnDeathScriptClear( wxMouseEvent& event )
    {
        m_DeathScript->SetLabel(wxT(""));
        m_DeathScript->SetToolTip(wxT("左键选择，右键清除"));
    }

    void OnSaveNpcIniFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("保存INI"),
                             exepath + wxT("ini\\npc\\"),
                             wxT(""),
                             INI_MASK,
                             wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if(filedlg.ShowModal() == wxID_OK)
        {
            NpcItem item;
            InitNpcItem(&item);
            AssignToNpcItem(&item);
            item.MapX = m_item->MapX;
            item.MapY = m_item->MapY;
            SaveIni(filedlg.GetPath(), &item);
        }
    }

    void OpenFile(wxString relatePath)
    {
        if(relatePath.IsEmpty())
        {
            wxMessageBox(wxT("请先选择文件"), wxT("消息"));
            return;
        }
        if(wxFileName::FileExists(exepath + relatePath))
        {
        	wxString program = wxT("explorer");
        	if(wxFileName(relatePath).GetExt() == wxT("txt") &&
				wxFileName::FileExists(exepath + wxT("JxqyScriptEditor.exe")))
				program = exepath + wxT("JxqyScriptEditor.exe");
            wxExecute(program + wxT(" \"")  + exepath + relatePath + wxT("\""));
        }
        else
        {
            wxMessageBox(relatePath + wxT("  文件不存在"), wxT("消息"));
            return;
        }
    }

    wxString INI_MASK,INI_MESSGEG, TXT_MASK;
    long STYLE;
    wxString exepath, m_mapName;
    AsfImgList *m_NpcAsfImgList;
    NpcItem *m_item;
};

class ObjItemEditDialog: public ObjItemEditDialogBase
{
public:

    ObjItemEditDialog(wxWindow *parent,
                      const wxString mapname,
                      AsfImgList *list,
                      ObjItem *objitem)
        :ObjItemEditDialogBase(parent)
    {
        exepath = wxStandardPaths::Get().GetExecutablePath();
        exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
        m_mapName = mapname;
        m_ObjAsfImgList = list;
        m_item = objitem;
    }
    virtual ~ObjItemEditDialog() {}
    void InitFromObjItem(ObjItem *item);
    void AssignToObjItem(ObjItem *item, bool onlySetted = false);
private:

    void OnObjFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("选择一个INI文件"),
                             exepath + wxT("ini\\objres\\"),
                             wxT(""),
                             wxT("INI文件(*.ini)|*.ini"),
                             wxFD_OPEN);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_ObjFile->SetLabel(filedlg.GetFilename());
            m_ObjFile->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnEditObjFile( wxCommandEvent& event )
    {
        wxString path = m_ObjFile->GetLabel();
        if(!path.IsEmpty()) path = wxT("ini\\objres\\") + path;
        OpenFile(path);
    }
    void OnScriptFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("选择一个脚本文件"),
                             exepath + wxT("script\\common\\"),
                             wxT(""),
                             wxT("TXT文件(*.txt)|*.txt"),
                             wxFD_OPEN);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_ScriptFile->SetLabel(filedlg.GetFilename());
            m_ScriptFile->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnClearObjFile( wxMouseEvent& event )
    {
    	m_ObjFile->SetLabel(wxT(""));
    	m_ObjFile->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnClearScriptFile( wxMouseEvent& event )
    {
        m_ScriptFile->SetLabel(wxT(""));
        m_ScriptFile->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnEditScriptFile( wxCommandEvent& event )
    {
        wxString path = m_ScriptFile->GetLabel();
        if(!path.IsEmpty())
        {
            if(wxFileName::FileExists(exepath + wxT("script\\common\\") + path))
                path = wxT("script\\common\\") + path;
            else
                path = wxT("script\\map\\") + m_mapName + wxT("\\") + path;
        }
        OpenFile(path);
    }
    void OnWavFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("选择声音文件"),
                             exepath + wxT("sound\\"),
                             wxT(""),
                             wxT("WAV文件(*.wav)|*.wav"),
                             wxFD_OPEN);

        if(filedlg.ShowModal() == wxID_OK)
        {
            m_WavFile->SetLabel(filedlg.GetFilename());
            m_WavFile->SetToolTip(filedlg.GetFilename());
        }
    }
    void OnResetValue( wxMouseEvent& event )
    {
        switch(event.GetId())
        {
		case MYID_OBJ_KIND:
			m_Kind->SetSelection(-1);
			break;
		case MYID_OBJ_DIR:
			m_Dir->SetSelection(-1);
			break;
        }
    }
    void OnClearWavFile( wxMouseEvent& event )
    {
        m_WavFile->SetLabel(wxT(""));
        m_WavFile->SetToolTip(wxT("左键选择，右键清除"));
    }
    void OnOpenWavFile( wxCommandEvent& event )
    {
        wxString path = m_WavFile->GetLabel();
        if(!path.IsEmpty()) path = wxT("sound\\") + path;
        OpenFile(path);
    }
    void OnOk( wxCommandEvent& event )
    {
        EndModal(wxID_OK);
    }
    void OnCancle( wxCommandEvent& event )
    {
        EndModal(wxID_CANCEL);
    }
    void OnSaveObjIniFile( wxCommandEvent& event )
    {
        wxFileDialog filedlg(this,
                             wxT("保存INI"),
                             exepath + wxT("ini\\obj\\"),
                             wxT(""),
                             wxT("INI文件(*.ini)|*.ini"),
                             wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if(filedlg.ShowModal() == wxID_OK)
        {
            ObjItem item;
            InitObjItem(&item);
            AssignToObjItem(&item);
            item.MapX = m_item->MapX;
            item.MapY = m_item->MapY;
            SaveIni(filedlg.GetPath(), NULL, &item);
        }
    }

    void OpenFile(wxString relatePath)
    {
        if(relatePath.IsEmpty())
        {
            wxMessageBox(wxT("请先选择文件"), wxT("消息"));
            return;
        }
        if(wxFileName::FileExists(exepath + relatePath))
        {
        	wxString program = wxT("explorer");
        	if(wxFileName(relatePath).GetExt() == wxT("txt") &&
				wxFileName::FileExists(exepath + wxT("JxqyScriptEditor.exe")))
				program = exepath + wxT("JxqyScriptEditor.exe");
            wxExecute(program + wxT(" \"")  + exepath + relatePath + wxT("\""));
        }
        else
        {
            wxMessageBox(relatePath + wxT("  文件不存在"), wxT("消息"));
            return;
        }
    }

    wxString exepath, m_mapName;
    AsfImgList *m_ObjAsfImgList;
    ObjItem *m_item;
};

class SetTmxHelperPortDialog : public SetTmxHelperPortDialogBase
{
public:
	SetTmxHelperPortDialog(wxWindow *parent)
	: SetTmxHelperPortDialogBase(parent)
	{
		SetTitle(wxT("设置端口"));
		m_spinCtrlPort->SetValue((int)TmxReaderSetting::GetPort());
	}

	virtual void OnButtonOKClick( wxCommandEvent& event )
	{
		if(m_spinCtrlPort->GetValue() != (int)TmxReaderSetting::GetPort())
		{
			TmxReaderSetting::SetPortValue((unsigned short)m_spinCtrlPort->GetValue());
			TmxReaderSetting::SaveConfig();
			wxMessageBox(wxT("设置成功，请重启程序！"));
		}
		EndModal(wxID_OK);
	}
};

#endif // MAPTOOL_H
