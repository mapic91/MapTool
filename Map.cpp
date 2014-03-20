#include "wx/filename.h"
#include "wx/stdpaths.h"
#include "Map.h"
#include "MpcDecode.hpp"
#include <fstream>

using namespace std;

Map::Map()
{
    //ctor
}

Map::~Map()
{
    //dtor
}

bool Map::ReadFile(const wxString FilePath)
{

    ifstream mapfile(FilePath.char_str(), ios_base::binary | ios_base::in);
    if(!mapfile.is_open()) return false;

    unsigned char temp4b[4];
    char tempc;
    wxString wx_mpcpath;

    //check file format correctly
    char vinfo[16];
    mapfile.read(vinfo, 16);
    if(strcmp(vinfo, "MAP File Ver2.0") != 0)
    {
        mapfile.close();
        return false;
    }

    //initialize mpc file path
    mapfile.seekg(16, ios_base::cur);
    char mpcpath[32] = {0};
    for(int pidx = 0; pidx < 32 ;pidx++)
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
    Name32b* mpcnames = (Name32b*)malloc(32 * 255);
    wxString wx_mpcpaths[255];
    for(int mi = 0; mi < 32 * 255; mi++)
    {
        ((char*)mpcnames)[mi] = 0;
    }
    for(int namei = 0; namei < 255; namei++)
    {
        for(int nc = 0; nc < 64; nc++)
        {
            mapfile.read(&tempc, 1);
            if(nc < 31 && tempc != 0) mpcnames[namei].Name[nc] = tempc;
        }
        wx_mpcpaths[namei] = wx_mpcpath + wxT("\\") + wxString(mpcnames[namei].Name);
    }
    free(mpcnames);

    long totaltiles = mCol * mRow;
    Tile* tiles = new Tile[totaltiles];
    mapfile.seekg(16512, ios_base::beg);
    for(long tilei = 0; tilei < totaltiles; tilei++)
    {
        mapfile.read((char*)(&tiles[tilei]), 8);
        mapfile.seekg(2, ios_base::cur);
    }

    mImg.Create(mPixelWidth, mPixelHeight, true);
    mImg.SetAlpha();

    //draw layer 1
    DrawLayer(1, tiles, wx_mpcpaths);
    DrawLayer(2, tiles, wx_mpcpaths);
    DrawLayer(3, tiles, wx_mpcpaths);
    return true;
}

void Map::DrawLayer(int index, Tile *tiles, wxString* mpcpaths)
{
    if(index < 1 || index > 3) return;

    MpcDecode decode;
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
        decode.ReadMpcFile(mpcpaths[mpcno - 1]);
        frmdata = decode.GetDecodedFrameData(frmno, &width, &height, MpcDecode::PIC_RGBA);
        DrawTile(tilei%mCol, tilei/mCol, width, height, frmdata);
        free(frmdata);
    }
}

void Map::DrawTile(long Column, long Row, long TileWidth, long TileHeight, unsigned char* TileData)
{
    if(TileData == NULL || Column > mCol || Row > mRow) return;

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
            if(TileData[datai + 3] != 0 && (graphx + wi) != 0 && (graphy + hi) != 0)
            {
                mImg.SetRGB(graphx + wi, graphy + hi,
                        TileData[datai],
                        TileData[datai + 1],
                        TileData[datai + 2]);
                mImg.SetAlpha(graphx + wi, graphy + hi, 0xFF);
            }

            datai += 4;
        }
    }
}













