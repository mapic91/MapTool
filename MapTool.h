#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>
#include "Map.h"
#include "Npc.hpp"
#include "AsfDecode.hpp"

#include "wx/dcbuffer.h"
#include "wx/filedlg.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"

#include <map>

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

class MapTool : public MapFrameBase
{
public:
    MapTool(wxWindow* parent);
    virtual ~MapTool();
protected:
private:

    enum MYID
    {
        ID_SHOWNPCKEY = 10000,
        ID_SHOWOBJKEY,
        ID_ATTRIBUTELISTITEM,
        ID_DELETELISTITEM,
        ID_SETFIXPOSUNDO,
        ID_SETFIXPOSCOMPLETE,
    };

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

    void ShowNpcItemEditor(long npcitemidx);
    void ShowObjItemEditor(long objitemidx);
    void NpcItemEditShowModle(NpcItemEditDialog *dialog, NpcItem *npcitem);

    //NPC
    void OnLoadCharater( wxCommandEvent& event );
    void OnPlaceMode( wxCommandEvent& event ) ;
    void OnCharacterDirection( wxCommandEvent& event );
    void OnImportNpcFile( wxCommandEvent& event );
    void OnOutputNpcFile( wxCommandEvent& event );
    void OnShowNpcCounts( wxCommandEvent& event )
    {
        wxMessageBox(wxString::Format(wxT("NPC 数量：Count = %d"), m_NpcList.getCounts()),
                     wxT("消息"));
    }
    void OnClearNpc( wxCommandEvent& event )
    {
        m_NpcList.Clear();
        FreeAsfImgList(m_NpcAsfImgList);
        RefreshNpcList();
    }

    //OBJ
    void OnLoadObject( wxCommandEvent& event );
    void OnObjectDirection( wxCommandEvent& event )
    {
        OnCharacterDirection(event);
    }
    void OnImportObjFile( wxCommandEvent& event );
    void OnOutputObjFile( wxCommandEvent& event );
    void OnShowObjCount( wxCommandEvent& event )
    {
        wxMessageBox(wxString::Format(wxT("OBJ 数量：Count = %d"), m_ObjList.getCounts()),
                     wxT("消息"));
    }
    void OnClearObj( wxCommandEvent& event )
    {
        m_ObjList.Clear();
        FreeAsfImgList(m_ObjAsfImgList);
        RefreshObjList();
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
    void OnListItemSelected( wxListEvent& event );
    void RefreshNpcList();
    void RefreshObjList();
    void ShowTile(int tileX, int tileY);
    void DeleteListItem( wxCommandEvent& event);
    void AttributeListItem( wxCommandEvent& event);
    void ShowAttributeListItem();
    void UpdateListItem(int index, int listType = NPCLIST);
    void UpdateListItem(NpcItem *item);
    void UpdateListItem(ObjItem *item);
    enum  {NPCLIST, OBJLIST};
    int m_curList, m_curListIndex;
    bool m_placeModeNotDraw;

    //currentView: if true dc is the current mapview client,
    // else dc is the entire map
    void DrawRectangle(long col, long row, wxDC &dc, bool currentView = true);
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
    void RedrawMapView();
    void CheckMapViewBeginPosition();

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
    NpcItem m_PlaceNpcData, *m_MoveNpcItem;
    ObjItem m_PlaceObjData, *m_MoveObjItem;
    NpcList m_NpcList;
    ObjList m_ObjList;
    AsfImgList *m_NpcAsfImgList, *m_ObjAsfImgList;
    wxString m_NpcIniFilePath, m_ObjIniFilePath;
    bool m_isObj, m_isNpc;

    //File dialog path
    wxString m_NpcObjPath;

    //FixPos
    void StartFixPosEdit(NpcItem *npcitem);
    void EndFixPosEdit(bool isCancle = false);
    void DrawEditFixPos(wxDC& dc);
    void CorrectFixedPos(NpcItem *item);
    bool m_isEditFixPos;
    NpcItem *m_fixPosEditItem;
    std::list<wxPoint> m_fixPosPoints;
    typedef std::list<wxPoint>::iterator FixPosListIterator;

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
    void AssignToNpcItem(NpcItem *item);
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
    void AssignToObjItem(ObjItem *item);
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

#endif // MAPTOOL_H
