#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>
#include "Map.h"


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

        wxImage* ReadMap();
        //own: true - img is deleted. false - img not deleted
        void SetMapView(wxImage* img, bool own = true);

        wxImage m_MapImg;
        Map map;
};

#endif // MAPTOOL_H
