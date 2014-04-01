#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"
#include "wx/log.h"
#include "Map.h"
#include <fstream>

using namespace std;

Map::Map()
{
    decode = NULL;
    tiles = NULL;
    LoadResource();
}

Map::~Map()
{
    DestoryData();
    DestoryResource();
    //dtor
}

bool Map::ReadFile(const wxString FilePath)
{
    DestoryData();

    ifstream mapfile(FilePath.char_str(), ios_base::binary | ios_base::in);
    if(!mapfile.is_open()) return false;

    unsigned char temp4b[4];
    char tempc;
    wxString wx_mpcpath;

    //check file format correctly
    char vinfo[16];
    mapfile.read(vinfo, 16);
    if(strncmp(vinfo, "MAP File Ver2.0", 12) != 0)
    {
        mapfile.close();
        return false;
    }

    //initialize mpc file path
    mapfile.seekg(16, ios_base::cur);
    char mpcpath[32] = {0};
    for(int pidx = 0; pidx < 32 ; pidx++)
    {
        mapfile.read(&tempc, 1);
        if(tempc != 0)
            mpcpath[pidx] = tempc;
    }
    if(mpcpath[0] == 0)
    {
        wx_mpcpath = wxT("\\mpc\\map\\") +
                     wxFileName::FileName(FilePath).GetName();
    }
    else
    {
        wx_mpcpath = wxString(mpcpath);
    }
    wxString exepath;
    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME);
    wx_mpcpath = exepath + wx_mpcpath;

    mapfile.seekg(4, ios_base::cur);
    mapfile.read((char*)temp4b, 4);
    mCol = Char2Long(temp4b);
    mPixelWidth = (mCol + 1) * 64 ;
    mapfile.read((char*)temp4b, 4);
    mRow = Char2Long(temp4b);
    mPixelHeight = (mRow + 1) * 16;

    mapfile.seekg(192, ios_base::beg);
    decode = new MpcDecode[255];
    char mpcname[32] = {0};
    for(int namei = 0; namei < 255; namei++)
    {
        for(int nc = 0; nc < 64; nc++)
        {
            mapfile.read(&tempc, 1);
            if(nc < 31)
            {
                mpcname[nc] = tempc;
            }
        }
        if(decode[namei].ReadMpcFile(wx_mpcpath + wxT("\\") + wxString(mpcname)))
        {
            decode[namei].BufferData();
        }
    }

    long totaltiles = mCol * mRow;
    tiles = new Tile[totaltiles];
    mapfile.seekg(16512, ios_base::beg);
    for(long tilei = 0; tilei < totaltiles; tilei++)
    {
        mapfile.read((char*)(&tiles[tilei]), 8);
        mapfile.seekg(2, ios_base::cur);
    }
    mapfile.close();

    return true;
}

void Map::DrawLayer(int index, Tile *tiles, MpcDecode *decode, wxImage *img)
{
    if(index < 1 || index > 3) return;
    if(tiles == NULL || decode == NULL || img == NULL) return;

    long totaltiles = mCol * mRow;
    long width, height;
    unsigned char mpcno, frmno, *frmdata;


    for(long tilei = 0; tilei < totaltiles; tilei++)
    {
        switch(index)
        {
        case 1:
            mpcno = tiles[tilei].fir_mpc_no;
            frmno = tiles[tilei].fir_frm_no;
            break;
        case 2:
            mpcno = tiles[tilei].sec_mpc_no;
            frmno = tiles[tilei].sec_frm_no;
            break;
        case 3:
            mpcno = tiles[tilei].thr_mpc_no;
            frmno = tiles[tilei].thr_frm_no;
            break;
        default:
            break;
        }
        if(mpcno == 0) continue;
        frmdata = decode[mpcno - 1].GetBuffedFrameData(frmno, &width, &height);
        DrawTile(tilei%mCol, tilei/mCol, width, height, frmdata, img);
    }
}

/**
    index: 1 - barrer, 2 - trap
**/
void Map::DrawLayer(int index, wxImage* img)
{
    if(index < 1|| index > 2) return;

    long totaltiles = mCol * mRow;
    long width, height;
    unsigned char barrertype, trapindex, *frmdata;

    for(long tilei = 0; tilei < totaltiles; tilei++)
    {
        frmdata = NULL;
        barrertype = 0;
        trapindex = 0;
        width = 0;
        height = 0;

        switch(index)
        {
        case 1: //barrer
            barrertype = tiles[tilei].barrer_type;
            break;
        case 2: //trap
            trapindex = tiles[tilei].trap_index;
            break;
        default:
            break;
        }

        if(barrertype != 0)
        {
            switch(barrertype)
            {
            case 0x40:
                width  = mBarrer[0].width;
                height = mBarrer[0].height;
                frmdata   = mBarrer[0].data;
                break;
            case 0x60:
                width  = mBarrer[1].width;
                height = mBarrer[1].height;
                frmdata   = mBarrer[1].data;
                break;
            case 0x80:
                width  = mBarrer[2].width;
                height = mBarrer[2].height;
                frmdata   = mBarrer[2].data;
                break;
            case 0xA0:
                width  = mBarrer[3].width;
                height = mBarrer[3].height;
                frmdata   = mBarrer[3].data;
                break;
            default:
                break;
            }
        }

        if(trapindex != 0 && trapindex <= 19)
        {
            width  = mTrap[trapindex - 1].width;
            height = mTrap[trapindex - 1].height;
            frmdata   = mTrap[trapindex - 1].data;
        }

        if(width == 0 || height == 0 || frmdata == NULL) continue;
        DrawTile(tilei%mCol, tilei/mCol, width, height, frmdata, img);
    }
}

void Map::GetTilePosition(int pixelX, int pixelY, int *tileX, int *tileY)
{
    if(tileX == NULL ||
       tileY == NULL ||
       pixelX < 0 ||
       pixelX > mPixelWidth ||
       pixelY < 0 ||
       pixelY > mPixelHeight) return;

    //first caculating even row position, considering each tile is 64 * 32 rectangle
    int nx, ny;
    nx = pixelX / 64;
    ny = (pixelY/ 32) * 2;

    //now caclue real position, please see »ñÈ¡µØÍ¼×ø±ê.jpg
    int dx, dy;
    dx = pixelX - nx * 64;
    dy = pixelY - (ny / 2) * 32;
    if(dx < 32)
    {
        if(dy < (32 - dx) / 2) // 1
        {
            nx = nx - 1;
            ny = ny - 1;
        }
        else if(dy > (dx / 2 + 16)) // 2
        {
            nx = nx - 1;
            ny = ny + 1;
        }
    }
    if(dx > 32)
    {
        if(dy < (dx - 32) / 2) //3
        {
            ny = ny - 1;
        }
        else if(dy > ((64 - dx) / 2 + 16)) // 4
        {
            ny = ny + 1;
        }
    }

    *tileX = nx;
    *tileY = ny;
}

void Map::DrawTile(long Column, long Row,
                   long TileWidth, long TileHeight,
                   unsigned char* TileData, wxImage *img)
{
    if(TileData == NULL || Column > mCol || Row > mRow || img == NULL) return;

    //tile top-left position
    long basex, basey;
    basex = (Row%2) * 32 + 64 * Column;
    basey = 16 * Row;

    //graph top-left position
    long graphx, graphy;
    graphx = basex + (32 - TileWidth/2);
    graphy = basey + (32 - TileHeight);

    unsigned long datai = 0;
    for(long hi = 0; hi < TileHeight; hi++)
    {
        for(long wi = 0; wi < TileWidth; wi++)
        {
            if(TileData[datai + 3] != 0)
            {
                img->SetRGB(graphx + wi, graphy + hi,
                            TileData[datai],
                            TileData[datai + 1],
                            TileData[datai + 2]);
                img->SetAlpha(graphx + wi, graphy + hi, 0xFF);
            }

            datai += 4;
        }
    }
}

wxImage* Map::getImage(unsigned char layer)
{
    wxImage *img = new wxImage;
    img->Create(mPixelWidth, mPixelHeight, true);
    img->SetAlpha();

    if(layer & LAYER1) DrawLayer(1, tiles, decode, img);
    if(layer & LAYER2) DrawLayer(2, tiles, decode, img);
    if(layer & LAYER3) DrawLayer(3, tiles, decode, img);
    if(layer & TRAP)   DrawLayer(2, img);
    if(layer & BARRER) DrawLayer(1, img);

    return img;
}

//wxImage* Map::getImage(int beginPosX, int beginPosY, int width, int height, unsigned char layer)
//{
//    wxImage *img = new wxImage;
//    img->Create(width, height, true);
//    img->SetAlpha();
//
//}

void Map::LoadResource()
{
    wxLogNull lognull;
    wxImage img;
    wxString exepath;
    exepath = wxStandardPaths::Get().GetExecutablePath();
    exepath = wxFileName::FileName(exepath).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR);
    exepath += wxT("resource\\");


    for(int i = 0; i < 19; i++)
    {
        img.LoadFile(exepath + wxString::Format(wxT("%d.png"), i + 1));
        AssignImgData(&mTrap[i], img);
    }

    img.LoadFile(exepath + wxT("Í¸.png"));
    AssignImgData(&mBarrer[0], img);
    img.LoadFile(exepath + wxT("ÌøÍ¸.png"));
    AssignImgData(&mBarrer[1], img);
    img.LoadFile(exepath + wxT("ÕÏ.png"));
    AssignImgData(&mBarrer[2], img);
    img.LoadFile(exepath + wxT("ÌøÕÏ.png"));
    AssignImgData(&mBarrer[3], img);
}

void Map::DestoryResource()
{
    for(int i = 0; i < 19; i++)
    {
        if(mTrap[i].data != NULL) free(mTrap[i].data);
    }

    for(int j = 0; j < 4; j++)
    {
        if(mBarrer[j].data != NULL) free(mBarrer[j].data);
    }
}

void Map::AssignImgData(ImgData *imgdata, wxImage &img)
{
    if(imgdata == NULL) return;

    if(img.IsOk())
    {
        unsigned char *pixdata, *alphadata;
        int width, height;
        long datasize;

        if(!img.HasAlpha()) img.SetAlpha();
        width = img.GetWidth();
        height = img.GetHeight();
        pixdata = img.GetData();
        alphadata = img.GetAlpha();
        datasize = (long) 4  * width * height;

        (*imgdata).width = (long)width;
        (*imgdata).height = (long)height;
        (*imgdata).data = (unsigned char*)malloc(datasize);
        if((*imgdata).data == NULL) return;

        int pi = 0, ai = 0;
        for(int di = 0; di < datasize;)
        {
            (*imgdata).data[di++] = pixdata[pi++];
            (*imgdata).data[di++] = pixdata[pi++];
            (*imgdata).data[di++] = pixdata[pi++];
            (*imgdata).data[di++] = alphadata[ai++];
        }
    }
    else
    {
        (*imgdata).width = 0;
        (*imgdata).height = 0;
        (*imgdata).data = NULL;
    }
}











