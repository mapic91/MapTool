#include "MapTool.h"
#include "wx/filedlg.h"
#include "wx/msgdlg.h"
#include "wx/dcmemory.h"
#include "wx/accel.h"
#include "wx/stdpaths.h"

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
    m_isPlaceMode = true;
    m_isDeleteMode = false;
    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

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
    this->SetTitle(wxString::Format(wxFormatString(wxT("%s      %ld × %ld - %ld × %ld")),
                                    filedlg.GetFilename(),
                                    map.getCol(),
                                    map.getRow(),
                                    map.getPixelWidth(),
                                    map.getPixelHeight()));

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
    wxPaintDC dc(m_MapView);
    wxMemoryDC memdc;
    int viewWidth, viewHeight, bmpWidth, bmpHeight;
    m_MapView->GetClientSize(&viewWidth, &viewHeight);
    bmpWidth = m_MapBitmap.GetWidth();
    bmpHeight = m_MapBitmap.GetHeight();

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

    int recposx, recposy;
    if(!map.GetPixelPosition(m_CurTileX, m_CurTileY, &recposx, &recposy)) return;

    dc.SetPen(*(wxThePenList->FindOrCreatePen(*wxGREEN)));
    wxPoint point[5];
    point[0] = wxPoint(32, 0);
    point[1] = wxPoint(64, 16);
    point[2] = wxPoint(32, 32);
    point[3] = wxPoint(0, 16);
    point[4] = wxPoint(32, 0);
    dc.DrawLines(5, point, recposx - m_ViewBeginx, recposy - m_ViewBeginy);

    if(m_isPlaceMode)
    {
        DrawTile(m_CurTileX, m_CurTileY, dc, &m_PlaceNpcData);
    }
}

void MapTool::DrawTile(long col, long row, wxPaintDC &dc, NpcItem *item)
{
    int recposx, recposy;
    if(!map.GetPixelPosition(col, row, &recposx, &recposy)) return;

    wxImage npcImg =
        item->NpcStand.GetDirectionImageFromBufferdData(item->Dir);
    if(npcImg.IsOk())
    {
        wxBitmap npcBmp(npcImg);

        long npcDrawX, npcDrawY, npcOffX, npcOffY;
        int npcWidth = npcImg.GetWidth();
        int npcHeight = npcImg.GetHeight();
        npcOffX = item->NpcStand.GetLeft();
        npcOffY = item->NpcStand.GetBottom();

        npcDrawX = recposx + 32 - npcOffX - m_ViewBeginx;
        npcDrawY = recposy + 64 - npcOffY + (32 - npcHeight) - m_ViewBeginy;

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
        if(!ReadNpcIni(exepath, m_NpcIniFilePath, item))
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
}
void MapTool::OnMouseMove( wxMouseEvent& event )
{
    long posx, posy;
    wxString msg;
    event.GetPosition(&posx, &posy);

    if(map.GetTilePosition(posx + m_ViewBeginx, posy + m_ViewBeginy, &m_CurTileX, &m_CurTileY))
        msg = wxString::Format(wxT("[%ld,%ld]"), m_CurTileX, m_CurTileY);

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

void MapTool::OnPlaceMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, true);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, false);

    m_isPlaceMode = true;
    m_isDeleteMode = false;

}
void MapTool::OnCharacterDirection( wxCommandEvent& event )
{
    m_PlaceNpcData.Dir++;
    if(m_PlaceNpcData.NpcStand.GetFramesCounts() != 0 && m_PlaceNpcData.NpcStand.GetDirection() != 0)
        m_PlaceNpcData.Dir %= m_PlaceNpcData.NpcStand.GetDirection();
    else
        m_PlaceNpcData.Dir %= 8;

    RedrawMapView();
}

void MapTool::OnDeleteMode( wxCommandEvent& event )
{
    m_ToolBarEdit->ToggleTool(ID_TOOLPLACE, false);
    m_ToolBarEdit->ToggleTool(ID_TOOLDELETE, true);

    m_isPlaceMode = false;
    m_isDeleteMode = true;

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
