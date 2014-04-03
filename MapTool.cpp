#include "MapTool.h"
#include "wx/msgdlg.h"
#include "wx/dcmemory.h"

#include "wx/accel.h"

using namespace std;

BEGIN_EVENT_TABLE(MapTool, MapFrameBase)
    EVT_MENU(ID_MAPUP, MapTool::OnMapUp)
    EVT_MENU(ID_MAPDOWN, MapTool::OnMapDown)
    EVT_MENU(ID_MAPLEFT, MapTool::OnMapLeft)
    EVT_MENU(ID_MAPRIGHT, MapTool::OnMapRight)
    EVT_MENU(ID_DIRECTION, MapTool::OnCharacterDirection)
END_EVENT_TABLE()


MapTool::MapTool(wxWindow* parent)
    :MapFrameBase(parent)
{
    m_ViewBeginx = m_ViewBeginy = 0;
    m_CurTileX = m_CurTileY = 0;
    m_PlaceNpcData.Dir = 0;
    m_PlaceNpcData.NpcStand = new AsfDecode;
    m_isPlaceMode = true;
    m_isDeleteMode = false;
    m_isEditAttribute = false;
    m_isMoveMode = false;
    m_AsfImgList = new AsfImgList;
    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    m_MoveNpcItem = NULL;

    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, true);
    this->SetTitle(wxT("剑侠情缘地图工具V1.1 - by 小试刀剑  2014.03.22"));
    this->SetIcon(wxICON(aaaa));
    this->SetSize(800, 600);
    this->Center();

    wxAcceleratorEntry ace[5];
    ace[0].Set(wxACCEL_NORMAL, WXK_UP, ID_MAPUP);
    ace[1].Set(wxACCEL_NORMAL, WXK_DOWN, ID_MAPDOWN);
    ace[2].Set(wxACCEL_NORMAL, WXK_LEFT, ID_MAPLEFT);
    ace[3].Set(wxACCEL_NORMAL, WXK_RIGHT, ID_MAPRIGHT);
    ace[4].Set(wxACCEL_NORMAL, WXK_SPACE, ID_DIRECTION);
    wxAcceleratorTable act(4, ace);
    SetAcceleratorTable(act);
}

MapTool::~MapTool()
{
    FreeAsfImgList(m_AsfImgList);
    if(m_AsfImgList != NULL) delete m_AsfImgList;
    if(m_PlaceNpcData.NpcStand != NULL) delete m_PlaceNpcData.NpcStand;
    //dtor
}

void MapTool::OpenMap(wxCommandEvent& event)
{
    wxFileDialog filedlg(this,
                         wxT("请选择一个地图文件"),
                         exepath + wxT("map\\"),
                         wxEmptyString,
                         wxT("MAP文件(*.map)|*.map"),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(filedlg.ShowModal() != wxID_OK) return;

    if(!map.ReadFile(filedlg.GetPath())) return;
    m_MapFileName = filedlg.GetFilename();
    this->SetTitle(wxString::Format(wxFormatString(wxT("%s      %ld × %ld - %ld × %ld")),
                                    m_MapFileName,
                                    map.getCol(),
                                    map.getRow(),
                                    map.getPixelWidth(),
                                    map.getPixelHeight()));
    //clear npcs
    m_NpcList.Clear();
    FreeAsfImgList(m_AsfImgList);

    m_MapView->Refresh(true);
    ReadMap();
    RedrawMapView();
}

void MapTool::SaveToPNG(wxCommandEvent& event)
{
    wxFileDialog filedlg(this,
                         wxT("存为PNG"),
                         wxEmptyString,
                         wxEmptyString,
                         wxT("MAP文件(*.png)|*.png"),
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if(filedlg.ShowModal() != wxID_OK) return;
    wxImage *img = ReadMap(true);
    img->SaveFile(filedlg.GetPath(), wxBITMAP_TYPE_PNG);
    delete img;

    wxMessageBox(wxT("完成"), wxT("消息"));
}

wxImage* MapTool::ReadMap(bool getImg)
{
    unsigned char layer = 0;
    if(m_Layer1->IsChecked()) layer |= Map::LAYER1;
    if(m_Layer2->IsChecked()) layer |= Map::LAYER2;
    if(m_Layer3->IsChecked()) layer |= Map::LAYER3;
    if(m_Barrer->IsChecked()) layer |= Map::BARRER;
    if(m_Trap->IsChecked())   layer |= Map::TRAP;

    wxImage *img = map.getImage(layer, m_LayerTransparent->IsChecked());
    m_MapBitmap = wxBitmap(*img);
    if(getImg) return img;
    else
    {
        delete img;
        return NULL;
    }
}

void MapTool::RedrawMapView()
{
    m_MapView->Refresh(false);
    m_MapView->Update();
    m_MapControl->Refresh(false);
    m_MapControl->Update();
}

void MapTool::OnMapDraw( wxPaintEvent& event )
{
    if(!m_MapBitmap.IsOk()) return;

    int viewWidth, viewHeight, bmpWidth, bmpHeight;
    m_MapView->GetClientSize(&viewWidth, &viewHeight);
    bmpWidth = m_MapBitmap.GetWidth();
    bmpHeight = m_MapBitmap.GetHeight();

    wxImage img(viewWidth, viewHeight, true);
    if(m_LayerTransparent->IsChecked()) img.SetAlpha();
    wxBitmap bufbitmap(img);
    img.Destroy();
    wxBufferedPaintDC dc(m_MapView, bufbitmap);
    wxMemoryDC memdc;

    if(m_ViewBeginx + viewWidth > bmpWidth) m_ViewBeginx = bmpWidth - viewWidth;
    if(m_ViewBeginy + viewHeight > bmpHeight) m_ViewBeginy = bmpHeight - viewHeight;
    if(m_ViewBeginx < 0) m_ViewBeginx = 0;
    if(m_ViewBeginy < 0) m_ViewBeginy = 0;

    //check whether bmp size is small than view
    if(m_ViewBeginx + viewWidth > bmpWidth) viewWidth = bmpWidth;
    if(m_ViewBeginy + viewHeight > bmpHeight) viewHeight = bmpHeight;

    memdc.SelectObject(m_MapBitmap);

    dc.Blit(0,
            0,
            viewWidth,
            viewHeight,
            &memdc,
            m_ViewBeginx,
            m_ViewBeginy,
            wxCOPY,
            m_LayerTransparent->IsChecked());

    //Draw reactangle current positon under mouse
    if(m_NpcList.HasItem(m_CurTileX, m_CurTileY))
        dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxYELLOW, 2)));
    else
        dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxGREEN)));
    DrawRectangle(m_CurTileX, m_CurTileY, dc);

    if(m_isPlaceMode)
    {
        DrawTile(m_CurTileX, m_CurTileY, dc, &m_PlaceNpcData);
    }

    DrawNpcs(dc);
}

void MapTool::DrawRectangle(long col, long row, wxBufferedPaintDC &dc)
{
    int recposx, recposy;
    if(map.GetPixelPosition(col, row, &recposx, &recposy))
    {
        wxPoint point[5];
        point[0] = wxPoint(32, 0);
        point[1] = wxPoint(64, 16);
        point[2] = wxPoint(32, 32);
        point[3] = wxPoint(0, 16);
        point[4] = wxPoint(32, 0);
        dc.DrawLines(5, point, recposx - m_ViewBeginx, recposy - m_ViewBeginy);
    }
}

void MapTool::DrawTile(long col, long row, wxBufferedPaintDC &dc, NpcItem *item)
{
    if(item == NULL) return;

    int recposx, recposy;
    if(!map.GetPixelPosition(col, row, &recposx, &recposy)) return;

    wxImage npcImg =
        item->NpcStand->GetDirectionImageFromBufferdData(item->Dir);
    if(npcImg.IsOk())
    {
        wxBitmap npcBmp(npcImg);

        long npcDrawX, npcDrawY, npcOffX, npcOffY;
        int npcWidth = npcImg.GetWidth();
        int npcHeight = npcImg.GetHeight();
        npcOffX = item->NpcStand->GetLeft();
        npcOffY = item->NpcStand->GetBottom();

        npcDrawX = recposx + 33 - npcOffX - m_ViewBeginx;
        npcDrawY = recposy + 58 - npcOffY + (32 - npcHeight) - m_ViewBeginy;

        wxMemoryDC memdc;
        memdc.SelectObject(npcBmp);

        dc.Blit(npcDrawX,
                npcDrawY,
                npcWidth,
                npcHeight,
                &memdc,
                0,
                0,
                wxCOPY,
                true);
    }
}

void MapTool::DrawNpcs(wxBufferedPaintDC &dc)
{
    int counts = m_NpcList.getCounts();
    NpcItem *item;
    for(int i = 0; i < counts; i++)
    {
        item = m_NpcList.GetItem(i);
        if(item == NULL) continue;
        DrawTile(item->MapX, item->MapY, dc, item);
    }
}

void MapTool::OnLayerTransparent( wxCommandEvent& event )
{
    ReadMap();
    m_MapView->Refresh(true);
    RedrawMapView();
}
void MapTool::OnLayer1( wxCommandEvent& event )
{
    ReadMap();
    RedrawMapView();
}
void MapTool::OnLayer2( wxCommandEvent& event )
{
    ReadMap();
    RedrawMapView();
}
void MapTool::OnLayer3( wxCommandEvent& event )
{
    ReadMap();
    RedrawMapView();
}
void MapTool::OnTrap( wxCommandEvent& event )
{
    ReadMap();
    RedrawMapView();
}
void MapTool::OnBarrer( wxCommandEvent& event )
{
    ReadMap();
    RedrawMapView();
}

void MapTool::OnMapUp( wxCommandEvent& event )
{
    m_ViewBeginy -= 16;
    CheckMapViewBeginPosition();
    RedrawMapView();
}
void MapTool::OnMapDown( wxCommandEvent& event )
{
    m_ViewBeginy += 16;
    CheckMapViewBeginPosition();
    RedrawMapView();
}
void MapTool::OnMapLeft( wxCommandEvent& event )
{
    m_ViewBeginx -= 64;
    CheckMapViewBeginPosition();
    RedrawMapView();
}
void MapTool::OnMapRight( wxCommandEvent& event )
{
    m_ViewBeginx += 64;
    CheckMapViewBeginPosition();
    RedrawMapView();
}
void MapTool::OnMapViewMouseLeftDown( wxMouseEvent& event )
{
    NpcItem *item = new NpcItem;
    if(m_isPlaceMode && !m_NpcIniFilePath.IsEmpty())
    {
        if(!ReadNpcIni(exepath, m_NpcIniFilePath, item, m_AsfImgList))
        {
            delete item;
            return;
        }
        item->MapX = m_CurTileX;
        item->MapY = m_CurTileY;
        item->Dir = m_PlaceNpcData.Dir;
        m_NpcList.DeleteItem(m_CurTileX, m_CurTileY);
        m_NpcList.AddItem(item);
    }
    else if(m_isDeleteMode)
    {
        m_NpcList.DeleteItem(m_CurTileX, m_CurTileY);
    }
    else if(m_isMoveMode)
    {
        m_MoveNpcItem = m_NpcList.GetItem(m_CurTileX, m_CurTileY);
    }
    else if(m_isEditAttribute)
    {
        NpcItem *item = m_NpcList.GetItem(m_CurTileX, m_CurTileY);
        if(item != NULL)
        {
            NpcItemEditDialog
                itemEdit(this, m_MapFileName.Mid(0, m_MapFileName.size() - 4), m_AsfImgList);
            itemEdit.InitFromNpcItem(item);
            if(itemEdit.ShowModal() == wxID_OK)
                itemEdit.AssignToNpcItem(item);
        }
    }
}
void MapTool::OnMapViewMouseLeftUp( wxMouseEvent& event )
{
    if(m_isMoveMode) m_MoveNpcItem = NULL;
}
void MapTool::OnMouseMove( wxMouseEvent& event )
{
    long posx, posy;
    wxString msg;
    event.GetPosition(&posx, &posy);

    if(map.GetTilePosition(posx + m_ViewBeginx, posy + m_ViewBeginy, &m_CurTileX, &m_CurTileY))
        msg = wxString::Format(wxT("[%ld,%ld]"), m_CurTileX, m_CurTileY);

    if(m_isMoveMode && m_MoveNpcItem != NULL && event.Dragging())
    {
        if(m_CurTileX >=0 && m_CurTileX < map.getCol())
            m_MoveNpcItem->MapX = m_CurTileX;
        if(m_CurTileY >= 0 && m_CurTileY < map.getRow())
            m_MoveNpcItem->MapY = m_CurTileY;
    }

    RedrawMapView();

    m_StatusBar->SetStatusText(msg, 0);
}

void MapTool::OnDrawMapControl( wxPaintEvent& event )
{
    int ctlwidth, ctlheight, mapviewwidth, mapviewheight, mapwidth, mapheight;

    m_MapControl->GetClientSize(&ctlwidth, &ctlheight);
    m_MapView->GetClientSize(&mapviewwidth, &mapviewheight);
    mapwidth = m_MapBitmap.GetWidth();
    mapheight = m_MapBitmap.GetHeight();

    if(mapwidth == 0 || mapheight == 0) return;

    wxMemoryDC memdc;
    memdc.SelectObject(m_MapBitmap);
    wxPaintDC dc(m_MapControl);

    dc.StretchBlit(0,
                   0,
                   ctlwidth,
                   ctlheight,
                   &memdc,
                   0,
                   0,
                   mapwidth,
                   mapheight
                  );

    int recwidth, recheight, recbegx, recbegy;
    recwidth = ctlwidth * mapviewwidth / mapwidth;
    recheight = ctlheight * mapviewheight / mapheight;
    recbegx = ctlwidth * m_ViewBeginx / mapwidth;
    recbegy = ctlheight * m_ViewBeginy / mapheight;
    wxPoint points[4];
    points[0] = wxPoint(0, 0);
    points[1] = wxPoint(recwidth, 0);
    points[2] = wxPoint(recwidth, recheight);
    points[3] = wxPoint(0, recheight);
    dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxRED)));
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawPolygon(4,
                   points,
                   recbegx,
                   recbegy);

}

void MapTool::OnMapCtrlMouseMotion( wxMouseEvent& event )
{
    if(!event.Dragging()) return;

    int ctlwidth, ctlheight,
        mapviewwidth, mapviewheight,
        mapwidth, mapheight,
        mousex, mousey;

    m_MapControl->GetClientSize(&ctlwidth, &ctlheight);
    m_MapView->GetClientSize(&mapviewwidth, &mapviewheight);
    mapwidth = m_MapBitmap.GetWidth();
    mapheight = m_MapBitmap.GetHeight();
    event.GetPosition(&mousex, &mousey);

    int recwidth, recheight, recbegx, recbegy;
    recwidth = ctlwidth * mapviewwidth / mapwidth;
    recheight = ctlheight * mapviewheight / mapheight;
    recbegx = mousex - recwidth / 2;
    recbegy = mousey - recheight /2;

    if(recbegx + recwidth > ctlwidth) recbegx = ctlwidth - recwidth;
    if(recbegy + recheight > ctlheight) recbegy = ctlheight - recheight;
    if(recbegx < 0) recbegx = 0;
    if(recbegy < 0) recbegy = 0;

    m_ViewBeginx = recbegx * mapwidth / ctlwidth;
    m_ViewBeginy = recbegy * mapheight / ctlheight;

    RedrawMapView();

}

void MapTool::OnLoadCharater( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("选择一个人物ini文件"),
                         exepath + wxT("ini\\npc\\"),
                         wxT(""),
                         wxT("ini文件(*.ini)|*.ini"),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(filedlg.ShowModal() != wxID_OK) return;
    m_NpcIniFilePath = filedlg.GetPath();
    ReadNpcIni(exepath, m_NpcIniFilePath, &m_PlaceNpcData);
}
void MapTool::OnCharacterDirection( wxCommandEvent& event )
{
    m_PlaceNpcData.Dir++;
    if(m_PlaceNpcData.NpcStand->GetFramesCounts() != 0 && m_PlaceNpcData.NpcStand->GetDirection() != 0)
        m_PlaceNpcData.Dir %= m_PlaceNpcData.NpcStand->GetDirection();
    else
        m_PlaceNpcData.Dir %= 8;

    RedrawMapView();
}
void MapTool::OnImportNpcFile( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("选择一个NPC文件"),
                         exepath + wxT("\\ini\\save\\"),
                         wxT(""),
                         wxT("NPC文件(*.npc)|*.npc"),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST
                         );

    if(filedlg.ShowModal() == wxID_OK)
    {
        if(NpcListImport(exepath, filedlg.GetPath(), &m_NpcList, m_AsfImgList))
        {
            wxMessageBox(wxT("完成"), wxT("消息"));
            RedrawMapView();
        }
        else
            wxMessageBox(wxT("失败"), wxT("错误"), wxOK | wxCENTER | wxICON_ERROR);
    }
}
void MapTool::OnOutputNpcFile( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("导出为NPC文件"),
                         exepath + wxT("\\ini\\save\\"),
                         wxT(""),
                         wxT("NPC文件(*.npc)|*.npc"),
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT
                         );

    if(filedlg.ShowModal() == wxID_OK)
    {
        if(NpcListSave(filedlg.GetPath(), m_MapFileName, &m_NpcList))
            wxMessageBox(wxT("完成"), wxT("消息"));
        else
            wxMessageBox(wxT("失败"), wxT("错误"), wxOK | wxCENTER | wxICON_ERROR);
    }
}
void MapTool::OnPlaceMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, true);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, false);
    m_ToolBarEdit->ToggleTool(ID_EDITATTRIBUTE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLMOVE, false);

    m_isPlaceMode = true;
    m_isDeleteMode = false;
    m_isEditAttribute = false;
    m_isMoveMode = false;
}
void MapTool::OnDeleteMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, true);
    m_ToolBarEdit->ToggleTool(ID_EDITATTRIBUTE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLMOVE, false);

    m_isPlaceMode = false;
    m_isDeleteMode = true;
    m_isEditAttribute = false;
    m_isMoveMode = false;
}
void MapTool::OnEditAttributeMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, false);
    m_ToolBarEdit->ToggleTool(ID_EDITATTRIBUTE, true);
    m_ToolBarEdit->ToggleTool(ID_TOOLMOVE, false);

    m_isPlaceMode = false;
    m_isDeleteMode = false;
    m_isEditAttribute = true;
    m_isMoveMode = false;
}
void MapTool::OnMoveMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, false);
    m_ToolBarEdit->ToggleTool(ID_EDITATTRIBUTE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLMOVE, true);

    m_isPlaceMode = false;
    m_isDeleteMode = false;
    m_isEditAttribute = false;
    m_isMoveMode = true;
}

void MapTool::CheckMapViewBeginPosition()
{
    int viewWidth, viewHeight, bmpWidth, bmpHeight;
    m_MapView->GetClientSize(&viewWidth, &viewHeight);
    bmpWidth = m_MapBitmap.GetWidth();
    bmpHeight = m_MapBitmap.GetHeight();
    if(m_ViewBeginx + viewWidth > bmpWidth) m_ViewBeginx = bmpWidth - viewWidth;
    if(m_ViewBeginy + viewHeight > bmpHeight) m_ViewBeginy = bmpHeight - viewHeight;
    if(m_ViewBeginx < 0) m_ViewBeginx = 0;
    if(m_ViewBeginy < 0) m_ViewBeginy = 0;
}

//////////////////////////////
///////////////////////////////
//NpcItemEditDialog

void NpcItemEditDialog::InitFromNpcItem(NpcItem *item)
{
    if(item == NULL) return;

    m_Name->ChangeValue(item->Name);
    if(item->Kind != -1)
        m_Kind->ChangeValue(wxString::Format(wxT("%d"), item->Kind));
    if(item->Relation != -1)
        m_Relation->ChangeValue(wxString::Format(wxT("%d"), item->Relation));
    if(item->PathFinder != -1)
        m_PathFinder->ChangeValue(wxString::Format(wxT("%d"), item->PathFinder));
    if(item->State != -1)
        m_State->ChangeValue(wxString::Format(wxT("%d"), item->State));
    if(item->VisionRadius != -1)
        m_VisionRadius->ChangeValue(wxString::Format(wxT("%d"), item->VisionRadius));
    if(item->DialogRadius != -1)
        m_DialogRadius->ChangeValue(wxString::Format(wxT("%d"), item->DialogRadius));
    if(item->AttackRadius != -1)
        m_AttackRadius->ChangeValue(wxString::Format(wxT("%d"), item->AttackRadius));
    if(item->Dir != -1)
        m_Dir->ChangeValue(wxString::Format(wxT("%d"), item->Dir));
    if(item->Lum != -1)
        m_Lum->ChangeValue(wxString::Format(wxT("%d"), item->Lum));
    if(item->Action != -1)
        m_Action->ChangeValue(wxString::Format(wxT("%d"), item->Action));
    if(item->WalkSpeed != -1)
        m_WalkSpeed->ChangeValue(wxString::Format(wxT("%d"), item->WalkSpeed));


    if(item->Evade != -1)
        m_Evade->ChangeValue(wxString::Format(wxT("%d"), item->Evade));
    if(item->Attack != -1)
        m_Attack->ChangeValue(wxString::Format(wxT("%d"), item->Attack));
    if(item->AttackLevel != -1)
        m_AttackLevel->ChangeValue(wxString::Format(wxT("%d"), item->AttackLevel));
    if(item->Defend != -1)
        m_Defend->ChangeValue(wxString::Format(wxT("%d"), item->Defend));
    if(item->Exp != -1)
        m_Exp->ChangeValue(wxString::Format(wxT("%d"), item->Exp));
    if(item->LevelUpExp != -1)
        m_LevelUpExp->ChangeValue(wxString::Format(wxT("%d"), item->LevelUpExp));
    if(item->Level != -1)
        m_Level->ChangeValue(wxString::Format(wxT("%d"), item->Level));
    if(item->Life != -1)
        m_Life->ChangeValue(wxString::Format(wxT("%d"), item->Life));
    if(item->LifeMax != -1)
        m_LifeMax->ChangeValue(wxString::Format(wxT("%d"), item->LifeMax));
    if(item->Thew != -1)
        m_Thew->ChangeValue(wxString::Format(wxT("%d"), item->Thew));
    if(item->ThewMax != -1)
        m_ThewMax->ChangeValue(wxString::Format(wxT("%d"), item->ThewMax));
    if(item->Mana != -1)
        m_Mana->ChangeValue(wxString::Format(wxT("%d"), item->Mana));
    if(item->ManaMax != -1)
        m_ManaMax->ChangeValue(wxString::Format(wxT("%d"), item->ManaMax));


    if(item->ExpBonus != -1)
        m_ExpBonus->ChangeValue(wxString::Format(wxT("%d"), item->ExpBonus));
    if(item->Idle != -1)
        m_Idle->ChangeValue(wxString::Format(wxT("%d"), item->Idle));

    if(!item->NpcIni.IsEmpty())
    {
        m_NpcIni->SetLabel(item->NpcIni);
        m_NpcIni->SetToolTip(item->NpcIni);
    }
    if(!item->BodyIni.IsEmpty())
    {
        m_BodyIni->SetLabel(item->BodyIni);
        m_BodyIni->SetToolTip(item->BodyIni);
    }
    if(!item->FlyIni.IsEmpty())
    {
        m_FlyIni->SetLabel(item->FlyIni);
        m_FlyIni->SetToolTip(item->FlyIni);
    }
    if(!item->FlyIni2.IsEmpty())
    {
        m_FlyIni2->SetLabel(item->FlyIni2);
        m_FlyIni2->SetToolTip(item->FlyIni2);
    }
    if(!item->ScriptFile.IsEmpty())
    {
        m_ScriptFile->SetLabel(item->ScriptFile);
        m_ScriptFile->SetToolTip(item->ScriptFile);
    }
    if(!item->DeathScript.IsEmpty())
    {
        m_DeathScript->SetLabel(item->DeathScript);
        m_DeathScript->SetToolTip(item->DeathScript);
    }
    if(!item->FixedPos.IsEmpty())
        m_FixedPos->SetValue(item->FixedPos);
}
void NpcItemEditDialog::AssignToNpcItem(NpcItem *item)
{
    if(item == NULL) return;
    wxString value;
    long n_val;

    value = m_Name->GetValue();
    if(!value.IsEmpty()) item->Name = value;

    value = m_Kind->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Kind = n_val;

    value = m_Relation->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Relation = n_val;

    value = m_PathFinder->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->PathFinder = n_val;

    value = m_State->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->State = n_val;

    value = m_VisionRadius->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->VisionRadius = n_val;

    value = m_DialogRadius->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->DialogRadius = n_val;

    value = m_AttackRadius->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->AttackRadius = n_val;

    value = m_Dir->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Dir = n_val;

    value = m_Lum->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Lum = n_val;

    value = m_Action->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Action = n_val;

    value = m_WalkSpeed->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->WalkSpeed = n_val;

    value = m_Evade->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Evade = n_val;

    value = m_Attack->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Attack = n_val;

    value = m_AttackLevel->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->AttackLevel = n_val;

    value = m_Defend->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Defend = n_val;

    value = m_Exp->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Exp = n_val;

    value = m_LevelUpExp->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->LevelUpExp = n_val;

    value = m_Level->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Level = n_val;

    value = m_Life->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Life = n_val;

    value = m_LifeMax->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->LifeMax = n_val;

    value = m_Thew->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Thew = n_val;

    value = m_ThewMax->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->ThewMax = n_val;

    value = m_Mana->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Mana = n_val;

    value = m_ManaMax->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->ManaMax = n_val;

    value = m_ExpBonus->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->ExpBonus = n_val;

    value = m_Idle->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Idle = n_val;


    value = m_NpcIni->GetLabel();
    item->NpcIni = value;
    FindAndBufferStandAsf(exepath, value, &(item->NpcStand), m_AsfImgList);
    value = m_BodyIni->GetLabel();
    item->BodyIni = value;
    value = m_FlyIni->GetLabel();
    item->FlyIni = value;
    value = m_FlyIni2->GetLabel();
    item->FlyIni2 = value;
    value = m_ScriptFile->GetLabel();
    item->ScriptFile = value;
    value = m_DeathScript->GetLabel();
    item->DeathScript = value;
    value = m_FixedPos->GetValue();
    item->FixedPos = value;
}
