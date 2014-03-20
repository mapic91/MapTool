#ifndef MAP_H
#define MAP_H

#include "wx/string.h"
#include "wx/image.h"

struct Name32b
{
    char Name[32];
};
struct Tile
{
    unsigned char fir_frm_no;
    unsigned char fir_mpc_no;
    unsigned char sec_frm_no;
    unsigned char sec_mpc_no;
    unsigned char thr_frm_no;
    unsigned char thr_mpc_no;
    unsigned char barrer_type;
    unsigned char trap_index;
};

class Map
{
    public:
        Map();
        virtual ~Map();

        long getCol() const {return mCol;}
        long getRow() const {return mRow;}
        long getPixelWidth() const {return mPixelWidth;}
        long getPixelHeight() const {return mPixelHeight;}
        wxImage getImage() const {return mImg;}

        bool ReadFile(const wxString FilePath);
    protected:
    private:

        /**
        Draw tile at Column,Row
        TileWidth: in tile width in pixel
        TileHeight: in tile height in pixel
        TileData: in tile pixel data
        **/
        void DrawTile(long Column, long Row, long TileWidth, long TileHeight, unsigned char* TileData);

        /**
        index: 1,3,4
        tiles: (mCol * mRow) tiles data
        mpcpaths: 255 mpc files path
        **/
        void DrawLayer(int index, Tile *tiles, wxString* mpcpaths);

        long Char2Long(const unsigned char* in) const
        {
            long temp = 0;
            temp |= (unsigned long)in[0];
            temp |= (((unsigned long)in[1]) << 8);
            temp |= (((unsigned long)in[2]) << 16);
            temp |= (((unsigned long)in[3]) << 24);
            return temp;
        }

        long mCol;
        long mRow;
        long mPixelWidth; // = (mCol + 1) * 64
        long mPixelHeight;// = (mRow + 1) * 16
        wxImage mImg;
};

#endif // MAP_H
