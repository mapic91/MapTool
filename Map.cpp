#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "wx/msgdlg.h"
#include "Map.h"
#include <fstream>

using namespace std;

Map::Map()
{
    decode = NULL;
    tiles = NULL;
    //ctor
}

Map::~Map()
{
    DestoryData();
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

    return img;
}













