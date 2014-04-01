#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>
#include "Map.h"
#include "Npc.hpp"
#include "AsfDecode.hpp"


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

		void OnMouseMove( wxMouseEvent& event );

        void OnLoadCharater( wxCommandEvent& event );

        void OnPlaceMode( wxCommandEvent& event ) ;
		void OnDeleteMode( wxCommandEvent& event );

        //if getImg is true return an image, else return NULL
        wxImage* ReadMap(bool getImg = false);
        void RedrawMapView();
        void CheckMapViewBeginPosition();

        wxBitmap m_MapBitmap;
        Map map;
        int m_ViewBeginx, m_ViewBeginy;
        //Current tile under mouse
        int m_CurTileX, m_CurTileY;
        wxString exepath;
        NpcData m_NpcData;
        AsfDecode m_PlaceNpc;
        char m_NpcCurrentDir;
        bool m_isPlaceMode, m_isDeleteMode;

        DECLARE_EVENT_TABLE()
};

#endif // MAPTOOL_H
