#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>


class MapTool : public MapFrameBase
{
    public:
        MapTool(wxWindow* parent);
        virtual ~MapTool();
    protected:
    private:
        void OpenMap(wxCommandEvent& event);
        void SaveToPNG(wxCommandEvent& event);

        wxImage m_MapImg;
};

#endif // MAPTOOL_H
