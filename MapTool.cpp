#include "MapTool.h"
#include "wx/filedlg.h"
#include "wx/msgdlg.h"
#include "wx/dcclient.h"
#include "wx/dcmemory.h"
#include "wx/accel.h"
#include "wx/stdpaths.h"

BEGIN_EVENT_TABLE(MapTool, MapFrameBase)
    EVT_MENU(ID_MAPUP, MapTool::OnMapUp)
    EVT_MENU(ID_MAPDOWN, MapTool::OnMapDown)
    EVT_MENU(ID_MAPLEFT, MapTool::OnMapLeft)
    EVT_MENU(ID_MAPRIGHT, MapTool::OnMapRight)
END_EVENT_TABLE()


MapTool::MapTool(wxWindow* parent)
    :MapFrameBase(parent)
{
    m_ViewBeginx = m_ViewBeginy = 0;
    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);

    this->SetTitle(wxT("剑侠情缘地图工具V1.1 - by 小试刀剑  2014.03.22"));
    this->SetIcon(wxICON(aaaa));
    this->SetSize(800, 600);
    this->Center();

    wxAcceleratorEntry ace[4];
    ace[0].Set(wxACCEL_NORMAL, WXK_UP, ID_MAPUP);
    ace[1].Set(wxACCEL_NORMAL, WXK_DOWN, ID_MAPDOWN);
    ace[2].Set(wxACCEL_NORMAL, WXK_LEFT, ID_MAPLEFT);
    ace[3].Set(wxACCEL_NORMAL, WXK_RIGHT, ID_MAPRIGHT);
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
    RefreshMapView();
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

void MapTool::RefreshMapView()
{
    m_MapView->Refresh(false);
    m_MapView->Update();
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
    return;
}

void MapTool::OnLayerTransparent( wxCommandEvent& event )
{
    ReadMap();
    m_MapView->Refresh(true);
    RefreshMapView();
}
void MapTool::OnLayer1( wxCommandEvent& event )
{
    ReadMap();
    RefreshMapView();
}
void MapTool::OnLayer2( wxCommandEvent& event )
{
    ReadMap();
    RefreshMapView();
}
void MapTool::OnLayer3( wxCommandEvent& event )
{
    ReadMap();
    RefreshMapView();
}
void MapTool::OnTrap( wxCommandEvent& event )
{
    ReadMap();
    RefreshMapView();
}
void MapTool::OnBarrer( wxCommandEvent& event )
{
    ReadMap();
    RefreshMapView();
}

void MapTool::OnMapUp( wxCommandEvent& event )
{
    m_ViewBeginy -= 16;
    CheckMapViewBeginPosition();
    RefreshMapView();
}
void MapTool::OnMapDown( wxCommandEvent& event )
{
    m_ViewBeginy += 16;
    CheckMapViewBeginPosition();
    RefreshMapView();
}
void MapTool::OnMapLeft( wxCommandEvent& event )
{
    m_ViewBeginx -= 64;
    CheckMapViewBeginPosition();
    RefreshMapView();
}
void MapTool::OnMapRight( wxCommandEvent& event )
{
    m_ViewBeginx += 64;
    CheckMapViewBeginPosition();
    RefreshMapView();
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
