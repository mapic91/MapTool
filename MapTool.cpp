#include "MapTool.h"
#include "wx/filedlg.h"
#include "wx/msgdlg.h"

#include "Map.h"

MapTool::MapTool(wxWindow* parent)
:MapFrameBase(parent)
{
    this->SetTitle(wxT("剑侠情缘地图工具V1.0 - by 小试刀剑"));
    this->SetIcon(wxICON(aaaa));
    this->SetSize(800, 600);
    this->Center();
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
    try
    {

        if(!map.ReadFile(filedlg.GetPath())) return;
    }
    catch(std::exception e)
    {
        wxMessageBox(wxT("Exception Occur!"));
    }

    m_MapPath->SetLabel(filedlg.GetPath());
    m_MapImg = map.getImage();

    m_MapView->SetBitmap(wxBitmap(m_MapImg));
    m_WindowMapView->FitInside();
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
    m_MapImg.SaveFile(filedlg.GetPath(), wxBITMAP_TYPE_PNG);
}
