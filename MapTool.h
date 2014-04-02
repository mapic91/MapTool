#ifndef MAPTOOL_H
#define MAPTOOL_H

#include <MapFrameBase.h>
#include "Map.h"
#include "Npc.hpp"
#include "AsfDecode.hpp"

#include "wx/dcbuffer.h"

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
		void OnMapViewMouseLeftDown( wxMouseEvent& event );

		void OnMouseMove( wxMouseEvent& event );

		void OnDrawMapControl( wxPaintEvent& event );
		void OnMapCtrlMouseMotion( wxMouseEvent& event );

        void OnLoadCharater( wxCommandEvent& event );
        void OnPlaceMode( wxCommandEvent& event ) ;
        void OnCharacterDirection( wxCommandEvent& event );
        void OnOutputNpcFile( wxCommandEvent& event );

		void OnDeleteMode( wxCommandEvent& event );

		void OnEditAttributeMode( wxCommandEvent& event );

        void DrawRectangle(long col, long row, wxBufferedPaintDC &dc);
		void DrawTile(long col, long row, wxBufferedPaintDC &dc, NpcItem *item);
		void DrawNpcs(wxBufferedPaintDC &dc);

        //if getImg is true return an image, else return NULL
        wxImage* ReadMap(bool getImg = false);
        void RedrawMapView();
        void CheckMapViewBeginPosition();

        wxBitmap m_MapBitmap;
        Map map;
        int m_ViewBeginx, m_ViewBeginy;
        //Current tile under mouse
        int m_CurTileX, m_CurTileY;
        wxString exepath, m_MapFileName;
        char m_NpcCurrentDir;
        bool m_isPlaceMode, m_isDeleteMode, m_isEditAttribute;

        //Npc list
        NpcItem m_PlaceNpcData;
        NpcList m_NpcList;
        wxString m_NpcIniFilePath;

        DECLARE_EVENT_TABLE()
};

class NpcItemEditDialog: public NpcItemEditDialogBase
{
public:
    NpcItemEditDialog(wxWindow *parent) :NpcItemEditDialogBase(parent){}
    virtual ~NpcItemEditDialog(){}
    void InitFromNpcItem(NpcItem *item);
    void AssignToNpcItem(NpcItem *item);
private:

    void OnOk( wxCommandEvent& event ){ EndModal(wxID_OK); }
	void OnCancle( wxCommandEvent& event ){ EndModal(wxID_CANCEL); }
};

#endif // MAPTOOL_H
