#include "MapTool.h"
#include "wx/filedlg.h"

#include "Map.h"

MapTool::MapTool(wxWindow* parent)
:MapFrameBase(parent)
{
    //ctor
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

    Map map;
    if(!map.ReadFile(filedlg.GetPath())) return;

    m_MapImg = map.getImage();

    m_MapView->SetBitmap(wxBitmap(m_MapImg));
    m_WindowMapView->FitInside();
}

void MapTool::SaveToPNG(wxCommandEvent& event)
{

}
