#ifndef MAP_H
#define MAP_H

#include "TmxMapMessage.pb.h"

#include "wx/string.h"
#include "wx/image.h"

#include "MpcDecode.hpp"
#include "TmxReaderClient.h"

struct Name32b
{
    char Name[32];
};
struct ImgData
{
    long width;
    long height;
    unsigned char *data; //RGBA

    ImgData()
		: width(0)
		, height(0)
		, data(nullptr)
    {

    }

    ~ImgData()
    {
    	Free();
    }

    void Free()
    {
    	if(data != nullptr)
    	{
			free(data);
			data = nullptr;
		};
		width=0;
		height=0;
    }
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

        enum MapType
        {
        	JXQY,
        	TMX
        };

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

        /**
        input: the map's pixel position
        output: the map's corresponding tile position(Column, Row)
        return: true - pixelX,pixelY is in map size range;
                false - out of map size, Column,Row is unchanged
        **/
        bool GetTilePosition(int pixelX, int pixelY, int *Column, int *Row);
        bool static GetTilePosition(int *Column, int *Row,int pixelX, int pixelY);
        /**
        input: the map's tile position
        output: the map's corresponding pixel position(top-left)
        return: true - Column,Row is in map size range;
                false - out of map size, pixelX,pixelY is unchanged
        **/
        bool GetPixelPosition(int Column, int Row, int *pixelX, int *pixelY);
        bool static GetPixelPosition(int *pixelX, int *pixelY,int Column, int Row);
        bool GetTileCenterPixelPosition(int Column, int Row, int *pixelX, int *pixelY);
        bool static GetTileCenterPixelPosition(int *pixelX, int *pixelY,int Column, int Row);

		bool IsTileBarrer(int tileX, int tileY);
		bool IsTileBarrerTrans(int tileX, int tileY);
		unsigned char GetTileBarrerCode(int tileX, int tileY);
		unsigned char GetTileBarrerCode(int index);
		unsigned char GetTrapIndex(int tileIndex);

        bool ReadFile(const wxString FilePath);

        void ShutDownClient();
    protected:
    private:

        /**
        Draw tile at Column,Row
        TileWidth: in tile width in pixel
        TileHeight: in tile height in pixel
        TileData: in tile pixel data
        **/
        void DrawTile(long Column, long Row, long TileWidth, long TileHeight, unsigned char* TileData, wxImage *img);

        void DrawTmxTile(long Column, long Row,const Proto::TmxMap::TileImg *tileImg, wxImage *img);

        void DoDrawLayer(int index, wxImage* img);

        /**
        index: 1,2,3
        tiles: (mCol * mRow) tiles data
        mpcpaths: 255 mpc files path
        **/
        void DrawLayer(int index, Tile *tiles, MpcDecode *decode, wxImage* img);

        void DrawTmxLayer(int index, wxImage* img);

        /**
        index: 1 - barrer, 2 - trap
        **/
        void DrawInfoLayer(int index, wxImage* img);

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
            if(mTmxImgSrc != nullptr) delete[] mTmxImgSrc;

            decode = NULL;
            tiles = NULL;
            mCol = mRow = mPixelWidth = mPixelHeight = 0;
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

        MapType mMapType;

        TmxReaderClient mTmxReadClient;
        Proto::TmxMap* mTmxMap;
        ImgData *mTmxImgSrc;
};

#endif // MAP_H
