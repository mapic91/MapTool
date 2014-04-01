#ifndef MAP_H
#define MAP_H

#include "wx/string.h"
#include "wx/image.h"

#include "MpcDecode.hpp"

struct Name32b
{
    char Name[32];
};
struct ImgData
{
    long width;
    long height;
    unsigned char *data; //RGBA
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

        static const unsigned char LAYER1 = 0x01;
        static const unsigned char LAYER2 = 0x02;
        static const unsigned char LAYER3 = 0x04;
        static const unsigned char BARRER = 0x08;
        static const unsigned char TRAP   = 0x10;

        long getCol() const {return mCol;}
        long getRow() const {return mRow;}
        long getPixelWidth() const {return mPixelWidth;}
        long getPixelHeight() const {return mPixelHeight;}
        //returned wxImage must deleted
        wxImage* getImage(unsigned char layer = LAYER1 | LAYER2 | LAYER3, bool hasAlpha = true);
       // wxImage* getImage(int beginPosX, int beginPosY, int width, int height, unsigned char layer = LAYER1 | LAYER2 | LAYER3);

        bool ReadFile(const wxString FilePath);
    protected:
    private:

        /**
        Draw tile at Column,Row
        TileWidth: in tile width in pixel
        TileHeight: in tile height in pixel
        TileData: in tile pixel data
        **/
        void DrawTile(long Column, long Row, long TileWidth, long TileHeight, unsigned char* TileData, wxImage *img);

        /**
        index: 1,2,3
        tiles: (mCol * mRow) tiles data
        mpcpaths: 255 mpc files path
        **/
        void DrawLayer(int index, Tile *tiles, MpcDecode *decode, wxImage* img);
        /**
        index: 1 - barrer, 2 - trap
        **/
        void DrawLayer(int index, wxImage* img);

        /**
        input: the map's pixel position
        output: the map's corresponding tile position(tileX, tileY)
        **/
        void GetTilePosition(int pixelX, int pixelY, int *tileX, int *tileY);

        long Char2Long(const unsigned char* in) const
        {
            long temp = 0;
            temp |= (unsigned long)in[0];
            temp |= (((unsigned long)in[1]) << 8);
            temp |= (((unsigned long)in[2]) << 16);
            temp |= (((unsigned long)in[3]) << 24);
            return temp;
        }

        void LoadResource();
        void DestoryResource();
        void AssignImgData(ImgData *imgdata, wxImage &img);

        void DestoryData()
        {
            if(decode != NULL) delete[] decode;
            if(tiles != NULL) delete[] tiles;

            decode = NULL;
            tiles = NULL;
        }

        long mCol;
        long mRow;
        long mPixelWidth; // = (mCol + 1) * 64
        long mPixelHeight;// = (mRow + 1) * 16
        MpcDecode *decode;
        Tile* tiles;
        //0 - Í¸ 1-ÌøÍ¸ 2-ÕÏ 3-ÌøÕÏ
        ImgData mBarrer[4];
        ImgData mTrap[19];
};

#endif // MAP_H
