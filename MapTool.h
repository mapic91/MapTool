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

class MapTool : public MapFrameBase
{
public:
    MapTool(wxWindow* parent);
    virtual ~MapTool();
protected:
private:
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

    void OnMouseMove( wxMouseEvent& event );

    void OnDrawMapControl( wxPaintEvent& event );
    void OnMapCtrlMouseMotion( wxMouseEvent& event );

    void OnLoadCharater( wxCommandEvent& event );
    void OnPlaceMode( wxCommandEvent& event ) ;
    void OnCharacterDirection( wxCommandEvent& event );
    void OnOutputNpcFile( wxCommandEvent& event );

    void OnDeleteMode( wxCommandEvent& event );

    void OnMoveMode( wxCommandEvent& event );

    void OnEditAttributeMode( wxCommandEvent& event );

    void DrawRectangle(long col, long row, wxBufferedPaintDC &dc);
    void DrawTile(long col, long row, wxBufferedPaintDC &dc, NpcItem *item);
    void DrawNpcs(wxBufferedPaintDC &dc);

    //if getImg is true return an image, else return NULL
    wxImage* ReadMap(bool getImg = false);
    void RedrawMapView();
    void CheckMapViewBeginPosition();

    wxBitmap m_MapBitmap;
    Map map;
    int m_ViewBeginx, m_ViewBeginy;
    //Current tile under mouse
    int m_CurTileX, m_CurTileY;
    wxString exepath, m_MapFileName;
    char m_NpcCurrentDir;
    bool m_isPlaceMode, m_isDeleteMode, m_isEditAttribute, m_isMoveMode;

    //Npc list
    NpcItem m_PlaceNpcData, *m_MoveNpcItem;
    NpcList m_NpcList;
    AsfImgList *m_AsfImgList;
    wxString m_NpcIniFilePath;

    DECLARE_EVENT_TABLE()
};

class NpcItemEditDialog: public NpcItemEditDialogBase
{
public:
    NpcItemEditDialog(wxWindow *parent,
                      const wxString mapname,
                      AsfImgList *list)
    :NpcItemEditDialogBase(parent)
    {
        exepath = wxStandardPaths::Get().GetExecutablePath();
        exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
        m_mapName = mapname;
        m_AsfImgList = list;

        INI_MASK = wxT("INI文件(*.ini)|*.ini");
        TXT_MASK = wxT("TXT文件(*.txt)|*.txt");
        INI_MESSGEG = wxT("选择一个INI文件");
        STYLE = wxFD_OPEN;
    }
    virtual ~NpcItemEditDialog() {}
    void InitFromNpcItem(NpcItem *item);
    void AssignToNpcItem(NpcItem *item);
private:

    void OnOk( wxCommandEvent& event )
    {
        EndModal(wxID_OK);
    }
    void OnCancle( wxCommandEvent& event )
    {
        EndModal(wxID_CANCEL);
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
                             INI_MESSGEG,
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
                             INI_MESSGEG,
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



    void OpenFile(wxString relatePath)
    {
        if(relatePath.IsEmpty())
        {
            wxMessageBox(wxT("请先选择文件"), wxT("消息"));
            return;
        }
        if(wxFileName::FileExists(exepath + relatePath))
        {
            wxExecute(wxT("explorer \"") + exepath + relatePath + wxT("\""));
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
    AsfImgList *m_AsfImgList;
};

#endif // MAPTOOL_H
