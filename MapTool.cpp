#include "MapTool.h"
#include "wx/filedlg.h"
#include "wx/msgdlg.h"

MapTool::MapTool(wxWindow* parent)
    :MapFrameBase(parent)
{
    this->SetTitle(wxT("剑侠情缘地图工具V1.0 - by 小试刀剑  2014.03.21"));
    this->SetIcon(wxICON(aaaa));
    this->SetSize(800, 600);
    this->Center();

    m_WindowMapView->SetScrollRate(10, 10);
}

MapTool::~MapTool()
{
    //dtor
}

void MapTool::OpenMap(wxCommandEvent& event)
{
    wxFileDialog filedlg(this,
                         wxT("请选择一个地图文件"),
                         wxEmptyString,
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
    SetMapView(ReadMap());
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
    wxImage *img = ReadMap();
    img->SaveFile(filedlg.GetPath(), wxBITMAP_TYPE_PNG);
    delete img;

    wxMessageBox(wxT("完成"), wxT("消息"));
}

wxImage* MapTool::ReadMap()
{
    unsigned char layer = 0;
    if(m_Layer1->IsChecked()) layer |= Map::LAYER1;
    if(m_Layer2->IsChecked()) layer |= Map::LAYER2;
    if(m_Layer3->IsChecked()) layer |= Map::LAYER3;
    if(m_Barrer->IsChecked()) layer |= Map::BARRER;
    if(m_Trap->IsChecked())   layer |= Map::TRAP;

    wxImage *img = map.getImage(layer);
    return img;
}

void MapTool::SetMapView(wxImage* img, bool owe)
{
    m_MapView->SetBitmap(wxBitmap(*img));
    m_WindowMapView->FitInside();
    if(owe) delete img;
}

void MapTool::OnLayer1( wxCommandEvent& event )
{
    SetMapView(ReadMap());
}
void MapTool::OnLayer2( wxCommandEvent& event )
{
    SetMapView(ReadMap());
}
void MapTool::OnLayer3( wxCommandEvent& event )
{
    SetMapView(ReadMap());
}
void MapTool::OnTrap( wxCommandEvent& event )
{
     SetMapView(ReadMap());
}
void MapTool::OnBarrer( wxCommandEvent& event )
{
     SetMapView(ReadMap());
}
