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
		void OnTrap( wxCommandEvent& event ) ;
		void OnBarrer( wxCommandEvent& event ) ;
		void OnMapDraw( wxPaintEvent& event ) ;
		void OnLayerTransparent( wxCommandEvent& event );

		void OnMapUp( wxCommandEvent& event ) ;
		void OnMapDown( wxCommandEvent& event ) ;
		void OnMapLeft( wxCommandEvent& event ) ;
		void OnMapRight( wxCommandEvent& event ) ;

        //if getImg is true return an image, else return NULL
        wxImage* ReadMap(bool getImg = false);
        void RefreshMapView();
        void CheckMapViewBeginPosition();

        wxBitmap m_MapBitmap;
        Map map;
        int m_ViewBeginx, m_ViewBeginy;
        wxString exepath;

        DECLARE_EVENT_TABLE()
};

#endif // MAPTOOL_H
