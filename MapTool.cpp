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
    m_PlaceObjData.Dir = 0;
    m_PlaceObjData.ObjCommon = new AsfDecode;
    m_isPlaceMode = true;
    m_isDeleteMode = false;
    m_isEditAttribute = false;
    m_isMoveMode = false;
    m_NpcAsfImgList = new AsfImgList;
    m_ObjAsfImgList = new AsfImgList;
    m_MoveNpcItem = NULL;
    m_MoveObjItem = NULL;
    m_isNpc = true;
    m_isObj = false;

    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, true);
    m_ToolBarEdit->ToggleTool(ID_NPCMODE, true);
    m_ToolBarEdit->ToggleTool(ID_SHOWNPC, true);
    m_ToolBarEdit->ToggleTool(ID_SHOWOBJ, true);

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
    if(m_NpcAsfImgList != NULL)
    {
        FreeAsfImgList(m_NpcAsfImgList);
        delete m_NpcAsfImgList;
    }
    if(m_ObjAsfImgList != NULL)
    {
        FreeAsfImgList(m_ObjAsfImgList);
        delete m_ObjAsfImgList;
    }
    if(m_PlaceNpcData.NpcStand != NULL) delete m_PlaceNpcData.NpcStand;
    if(m_PlaceObjData.ObjCommon != NULL) delete m_PlaceObjData.ObjCommon;
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
    m_ObjList.Clear();
    FreeAsfImgList(m_NpcAsfImgList);
    FreeAsfImgList(m_ObjAsfImgList);

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
                         wxT("PNG文件(*.png)|*.png"),
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

    //Draw Map
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
    if(m_NpcList.HasItem(m_CurTileX, m_CurTileY) ||
            m_ObjList.HasItem(m_CurTileX, m_CurTileY))
        dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxYELLOW, 2)));
    else
        dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxGREEN)));
    DrawRectangle(m_CurTileX, m_CurTileY, dc);

    //Draw PlaceMode
    if(m_isPlaceMode)
    {
        if(m_isNpc)
            DrawTile(m_CurTileX, m_CurTileY, dc, &m_PlaceNpcData);
        else if(m_isObj)
            DrawTile(m_CurTileX, m_CurTileY, dc, NULL, &m_PlaceObjData);
    }

    DrawObjsNpcs(dc);

    //Draw npc obj position
    if(!m_isPlaceMode)
    {
        if(m_isNpc && m_ToolBarEdit->GetToolState(ID_SHOWNPC))
        {
            dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxBLUE, 2)));
            DrawNpcPosition(dc);
        }
        if(m_isObj && m_ToolBarEdit->GetToolState(ID_SHOWOBJ))
        {
            dc.SetPen(*(wxThePenList->FindOrCreatePen(wxColour(0, 255, 255), 2)));
            DrawObjPostion(dc);
        }
    }
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

void MapTool::DrawTile(long col, long row, wxBufferedPaintDC &dc, NpcItem *npcitem, ObjItem *objitem)
{
    if(npcitem == NULL && objitem == NULL) return;

    int recposx, recposy;
    if(!map.GetPixelPosition(col, row, &recposx, &recposy)) return;

    long tDrawX = 0, tDrawY = 0, tOffX = 0, tOffY = 0;
    wxImage timg;
    int tWidth = 0, tHeight = 0;
    if(npcitem != NULL)
    {
        timg = npcitem->NpcStand->GetDirectionImageFromBufferdData(npcitem->Dir);
        tWidth = timg.GetWidth();
        tHeight = timg.GetHeight();
        tOffX = npcitem->NpcStand->GetLeft();
        tOffY = npcitem->NpcStand->GetBottom();

        //tDrawX = recposx + 33 - tOffX - m_ViewBeginx;
        //tDrawY = recposy + 58 - tOffY + (32 - tHeight) - m_ViewBeginy;

        //Tile beg at tile middle(32,16)
        tDrawX = recposx + 32 - tOffX - m_ViewBeginx;
        tDrawY = recposy + 16 - tOffY - m_ViewBeginy;
    }
    else if(objitem != NULL)
    {
        timg = objitem->ObjCommon->GetDirectionImageFromBufferdData(objitem->Dir);
        tWidth = timg.GetWidth();
        tHeight = timg.GetHeight();
        tOffX = objitem->ObjCommon->GetLeft();
        tOffY = objitem->ObjCommon->GetBottom();

        //Tile beg at tile middle(32,16)
        tDrawX = recposx + 32 - tOffX + objitem->OffX - m_ViewBeginx;
        tDrawY = recposy + 16 - tOffY + objitem->OffY - m_ViewBeginy;
    }

    if(timg.IsOk())
    {
        wxBitmap tbmp(timg);

        wxMemoryDC memdc;
        memdc.SelectObject(tbmp);

        dc.Blit(tDrawX,
                tDrawY,
                tWidth,
                tHeight,
                &memdc,
                0,
                0,
                wxCOPY,
                true);
    }
}

void MapTool::DrawObjsNpcs(wxBufferedPaintDC &dc)
{
    int counts;
    if(m_ToolBarEdit->GetToolState(ID_SHOWNPC))
    {
        counts = m_NpcList.getCounts();
        NpcItem *npcitem;
        for(int i = 0; i < counts; i++)
        {
            npcitem = m_NpcList.GetItem(i);
            if(npcitem == NULL) continue;
            DrawTile(npcitem->MapX, npcitem->MapY, dc, npcitem);
        }
    }

    if(m_ToolBarEdit->GetToolState(ID_SHOWOBJ))
    {
        counts = m_ObjList.getCounts();
        ObjItem *objitem;
        for(int j = 0; j < counts; j++)
        {
            objitem = m_ObjList.GetItem(j);
            if(objitem == NULL) continue;
            DrawTile(objitem->MapX, objitem->MapY, dc, NULL, objitem);
        }
    }

}

void MapTool::DrawNpcPosition(wxBufferedPaintDC &dc)
{
    int counts = m_NpcList.getCounts();
    NpcItem *npcitem;
    for(int i = 0; i < counts; i++)
    {
        npcitem = m_NpcList.GetItem(i);
        if(npcitem == NULL) continue;
        DrawRectangle(npcitem->MapX, npcitem->MapY, dc);
    }
}

void MapTool::DrawObjPostion(wxBufferedPaintDC &dc)
{
    int counts = m_ObjList.getCounts();
    ObjItem *objitem;
    for(int j = 0; j < counts; j++)
    {
        objitem = m_ObjList.GetItem(j);
        if(objitem == NULL) continue;
        DrawRectangle(objitem->MapX, objitem->MapY, dc);
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

    if(m_isPlaceMode)
    {
        if(m_isNpc && !m_NpcIniFilePath.IsEmpty())
        {
            NpcItem *npcitem = new NpcItem;
            if(!ReadIni(exepath, m_NpcIniFilePath,  npcitem, NULL, m_NpcAsfImgList))
            {
                delete npcitem;
                return;
            }
            npcitem->MapX = m_CurTileX;
            npcitem->MapY = m_CurTileY;
            npcitem->Dir = m_PlaceNpcData.Dir;
            m_NpcList.DeleteItem(m_CurTileX, m_CurTileY);
            m_NpcList.AddItem(npcitem);
        }
        else if(m_isObj && !m_ObjIniFilePath.IsEmpty())
        {
            ObjItem *objitem = new ObjItem;
            if(!ReadIni(exepath, m_ObjIniFilePath, NULL, objitem, m_ObjAsfImgList))
            {
                delete objitem;
                return;
            }
            objitem->MapX = m_CurTileX;
            objitem->MapY = m_CurTileY;
            objitem->Dir = m_PlaceObjData.Dir;
            m_ObjList.DeleteItem(m_CurTileX, m_CurTileY);
            m_ObjList.AddItem(objitem);
        }
    }
    else if(m_isDeleteMode)
    {
        if(m_isNpc)
            m_NpcList.DeleteItem(m_CurTileX, m_CurTileY);
        else if(m_isObj)
            m_ObjList.DeleteItem(m_CurTileX, m_CurTileY);
    }
    else if(m_isMoveMode)
    {
        if(m_isNpc)
            m_MoveNpcItem = m_NpcList.GetItem(m_CurTileX, m_CurTileY);
        else if(m_isObj)
            m_MoveObjItem = m_ObjList.GetItem(m_CurTileX, m_CurTileY);
    }
    else if(m_isEditAttribute)
    {
        if(m_isNpc)
        {
            NpcItem *npcitem = m_NpcList.GetItem(m_CurTileX, m_CurTileY);
            if(npcitem != NULL)
            {
                NpcItemEditDialog
                itemEdit(this, m_MapFileName.Mid(0, m_MapFileName.size() - 4), m_NpcAsfImgList, npcitem);
                itemEdit.InitFromNpcItem(npcitem);
                if(itemEdit.ShowModal() == wxID_OK)
                    itemEdit.AssignToNpcItem(npcitem);
            }
        }
        else if(m_isObj)
        {
            ObjItem *objitem = m_ObjList.GetItem(m_CurTileX, m_CurTileY);
            if(objitem != NULL)
            {
                ObjItemEditDialog
                itemEdit(this, m_MapFileName.Mid(0, m_MapFileName.size() - 4), m_ObjAsfImgList, objitem);
                itemEdit.InitFromObjItem(objitem);
                if(itemEdit.ShowModal() == wxID_OK)
                    itemEdit.AssignToObjItem(objitem);
            }
        }

    }
}
void MapTool::OnMapViewMouseLeftUp( wxMouseEvent& event )
{
    if(m_isMoveMode)
    {
        if(m_isNpc)
            m_MoveNpcItem = NULL;
        else if(m_isObj)
            m_MoveObjItem = NULL;
    }
}
void MapTool::OnMouseMove( wxMouseEvent& event )
{
    long posx, posy;
    wxString msg;
    event.GetPosition(&posx, &posy);

    if(map.GetTilePosition(posx + m_ViewBeginx, posy + m_ViewBeginy, &m_CurTileX, &m_CurTileY))
        msg = wxString::Format(wxT("[%ld,%ld]"), m_CurTileX, m_CurTileY);

    if(m_isMoveMode && event.Dragging())
    {
        if(m_CurTileX >=0 && m_CurTileX < map.getCol())
        {
            if(m_isNpc && m_MoveNpcItem != NULL)
                m_MoveNpcItem->MapX = m_CurTileX;
            else if(m_isObj && m_MoveObjItem != NULL)
                m_MoveObjItem->MapX = m_CurTileX;
        }
        if(m_CurTileY >= 0 && m_CurTileY < map.getRow())
        {
            if(m_isNpc && m_MoveNpcItem != NULL)
                m_MoveNpcItem->MapY = m_CurTileY;
            else if(m_isObj && m_MoveObjItem != NULL)
                m_MoveObjItem->MapY = m_CurTileY;
        }
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
    ReadIni(exepath, m_NpcIniFilePath, &m_PlaceNpcData);
}
void MapTool::OnLoadObject( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("选择一个物品ini文件"),
                         exepath + wxT("ini\\obj\\"),
                         wxT(""),
                         wxT("ini文件(*.ini)|*.ini"),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if(filedlg.ShowModal() != wxID_OK) return;
    m_ObjIniFilePath = filedlg.GetPath();
    ReadIni(exepath, m_ObjIniFilePath, NULL, &m_PlaceObjData);
}
void MapTool::OnCharacterDirection( wxCommandEvent& event )
{
    if(m_isNpc)
    {
        m_PlaceNpcData.Dir++;
        if(m_PlaceNpcData.NpcStand->GetFramesCounts() != 0 && m_PlaceNpcData.NpcStand->GetDirection() != 0)
            m_PlaceNpcData.Dir %= m_PlaceNpcData.NpcStand->GetDirection();
        else
            m_PlaceNpcData.Dir %= 8;
    }
    else if(m_isObj)
    {
        m_PlaceObjData.Dir++;
        if(m_PlaceObjData.ObjCommon->GetFramesCounts() != 0 && m_PlaceObjData.ObjCommon->GetDirection() != 0)
            m_PlaceObjData.Dir %= m_PlaceObjData.ObjCommon->GetDirection();
        else
            m_PlaceObjData.Dir %= 8;
    }

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
        if(NpcListImport(exepath, filedlg.GetPath(), &m_NpcList, m_NpcAsfImgList))
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
void MapTool::OnImportObjFile( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("选择一个OBJ文件"),
                         exepath + wxT("\\ini\\save\\"),
                         wxT(""),
                         wxT("OBJ文件(*.obj)|*.obj"),
                         wxFD_OPEN | wxFD_FILE_MUST_EXIST
                        );

    if(filedlg.ShowModal() == wxID_OK)
    {
        if(ObjListImport(exepath, filedlg.GetPath(), &m_ObjList, m_ObjAsfImgList))
        {
            wxMessageBox(wxT("完成"), wxT("消息"));
            RedrawMapView();
        }
        else
            wxMessageBox(wxT("失败"), wxT("错误"), wxOK | wxCENTER | wxICON_ERROR);
    }
}
void MapTool::OnOutputObjFile( wxCommandEvent& event )
{
    wxFileDialog filedlg(this,
                         wxT("导出为OBJ文件"),
                         exepath + wxT("\\ini\\save\\"),
                         wxT(""),
                         wxT("OBJ文件(*.obj)|*.obj"),
                         wxFD_SAVE | wxFD_OVERWRITE_PROMPT
                        );

    if(filedlg.ShowModal() == wxID_OK)
    {
        if(ObjListSave(filedlg.GetPath(), m_MapFileName, &m_ObjList))
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

    RedrawMapView();
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

    RedrawMapView();
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

    RedrawMapView();
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

    RedrawMapView();
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
    FindAndBufferAsf(exepath, value, wxT("[Stand]"), &(item->NpcStand), m_NpcAsfImgList);
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

void ObjItemEditDialog::InitFromObjItem(ObjItem *item)
{
    if(item == NULL) return;

    m_ObjName->ChangeValue(item->ObjName);
    if(item->Kind != -1)
        m_Kind->ChangeValue(wxString::Format(wxT("%d"), item->Kind));
    if(item->Dir != -1)
        m_Dir->ChangeValue(wxString::Format(wxT("%d"), item->Dir));
    if(item->Damage != -1)
        m_Damage->ChangeValue(wxString::Format(wxT("%d"), item->Damage));
    if(item->Frame != -1)
        m_Frame->ChangeValue(wxString::Format(wxT("%d"), item->Frame));
    if(item->Height != -1)
        m_Height->ChangeValue(wxString::Format(wxT("%d"), item->Height));
    if(item->Lum != -1)
        m_Lum->ChangeValue(wxString::Format(wxT("%d"), item->Lum));
    if(item->OffX != -1)
        m_OffX->ChangeValue(wxString::Format(wxT("%d"), item->OffX));
    if(item->OffY != -1)
        m_OffY->ChangeValue(wxString::Format(wxT("%d"), item->OffY));

    if(!item->ObjFile.IsEmpty())
    {
        m_ObjFile->SetLabel(item->ObjFile);
        m_ObjFile->SetToolTip(item->ObjFile);
    }
    if(!item->ScriptFile.IsEmpty())
    {
        m_ScriptFile->SetLabel(item->ScriptFile);
        m_ScriptFile->SetToolTip(item->ScriptFile);
    }
    if(!item->WavFile.IsEmpty())
    {
        m_WavFile->SetLabel(item->WavFile);
        m_WavFile->SetToolTip(item->WavFile);
    }
}
void ObjItemEditDialog::AssignToObjItem(ObjItem *item)
{
    if(item == NULL) return;

    wxString value;
    long n_val;

    value = m_ObjName->GetValue();
    if(!value.IsEmpty()) item->ObjName = value;

    value = m_Kind->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Kind = n_val;

    value = m_Dir->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Dir = n_val;

    value = m_Damage->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Damage = n_val;

    value = m_Frame->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Frame = n_val;

    value = m_Height->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Height = n_val;

    value = m_Lum->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->Lum = n_val;

    value = m_OffX->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->OffX = n_val;

    value = m_OffY->GetValue();
    if(!value.ToLong(&n_val)) n_val = -1;
    item->OffY = n_val;

    value = m_ObjFile->GetLabel();
    item->ObjFile = value;
    FindAndBufferAsf(exepath, value, wxT("[Common]"), &(item->ObjCommon), m_ObjAsfImgList);

    value = m_ScriptFile->GetLabel();
    item->ScriptFile = value;

    value = m_WavFile->GetLabel();
    item->WavFile = value;

}







