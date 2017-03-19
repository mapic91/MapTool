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
    mCol = mRow = mPixelWidth = mPixelHeight = 0;
    mTmxImgSrc = nullptr;
    mTmxMap = nullptr;
    LoadResource();
}

Map::~Map()
{
    DestoryData();
    DestoryResource();
    ShutDownClient();
}

bool Map::ReadFile(const wxString FilePath)
{
	wxFileName fileName(FilePath);
	wxString ext = fileName.GetExt().MakeLower();

	DestoryData();

	if(ext == "map")
	{
		mMapType = JXQY;
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
	else if(ext == "tmx")
	{
		mMapType = TMX;
		mTmxMap = mTmxReadClient.GetTmxMap(FilePath);

		if(mTmxMap != nullptr)
		{
			mCol = mTmxMap->width();
			mPixelWidth = (mCol + 1) * 64 ;
			mRow = mTmxMap->height();
			mPixelHeight = (mRow + 1) * 16;

			if(mTmxMap->imgs_size() > 0)
			{
				wxLogNull lognull;
				wxImage img;
				mTmxImgSrc = new ImgData[mTmxMap->imgs_size()];
				for(int i = 0; i < mTmxMap->imgs_size(); i++)
				{
					img.LoadFile(wxString::FromUTF8(mTmxMap->imgs(i).path().c_str()));
					AssignImgData(mTmxImgSrc+i, img);
				}
			}
		}

		return mTmxMap != nullptr;
	}
	return false;
}

void Map::ShutDownClient()
{
	mTmxReadClient.Exit();
}


void Map::DoDrawLayer(int index, wxImage* img)
{
	if(mMapType == JXQY)
	{
		DrawLayer(index, tiles, decode, img);
	}
	else if (mMapType == TMX)
	{
		DrawTmxLayer(index, img);
	}
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

void Map::DrawTmxLayer(int index, wxImage* img)
{
	if(index < 1 || index > 3) return;
	if(mTmxImgSrc == nullptr || mTmxMap == nullptr) return;

	index--;


	const Proto::TmxMap_Layer *layer = nullptr;
	for(int i = 0; i < mTmxMap->layers_size(); i++)
	{
		if(mTmxMap->layers(i).index() == index)
		{
			layer = &mTmxMap->layers(i);
			break;
		}
	}

	if(layer != nullptr)
	{
		int totaltiles = mCol * mRow;
		for(int i = 0; i < totaltiles; i++)
		{
			const Proto::TmxMap_TileInfo *info = &layer->tiles(i);
			if(info->gid() == 0) continue;
			DrawTmxTile(i%mCol, i/mCol, &mTmxMap->tileimgs(info->gid()), img);
		}
	}
}


/**
    index: 1 - barrer, 2 - trap
**/
void Map::DrawInfoLayer(int index, wxImage* img)
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
            barrertype = GetTileBarrerCode(tilei);
            break;
        case 2: //trap
            trapindex = GetTrapIndex(tilei);
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

unsigned char Map::GetTileBarrerCode(int tileX, int tileY)
{
    if(tileX < 0 || tileX > mCol ||
            tileY < 0 || tileY > mRow) return 0;

	int index = tileY * mCol + tileX;
	return GetTileBarrerCode(index);
}

unsigned char Map::GetTileBarrerCode(int index)
{
	char code = 0;
	if(index < mCol * mRow)
	{
		if(mMapType == JXQY)
		{
			code = tiles[index].barrer_type;
		}
		else if(mMapType == TMX)
		{
			Proto::TmxMap::BarrerType type = mTmxMap->settings(index).barrertype();
			switch(type)
			{
			case Proto::TmxMap::None:
				code = 0;
				break;
			case Proto::TmxMap::Obstacle:
				code = 0x80;
				break;
			case Proto::TmxMap::CanOverObstacle:
				code = 0xA0;
				break;
			case Proto::TmxMap::Trans:
				code = 0x40;
				break;
			case Proto::TmxMap::CanOVerTrans:
				code = 0x60;
				break;
			}
		}
	}
    return code;
}

unsigned char Map::GetTrapIndex(int tileIndex)
{
	unsigned char index = 0;
	if(tileIndex < mCol * mRow)
	{
		if(mMapType == JXQY)
		{
			index = tiles[tileIndex].trap_index;
		}
		else if(mMapType == TMX)
		{
			index = (unsigned char)mTmxMap->settings(tileIndex).trapindex();
		}
	}

    return index;
}

bool Map::IsObstacleForCharacter(int tileX, int tileY)
{
	int index = tileY * mCol + tileX;
	if (index >= 0 and index < mCol * mRow)
	{
		unsigned char code = GetTileBarrerCode(index);
		if ((code & (0x80 + 0x40)) == 0)
                    return false;
	}
	return true;
}

bool Map::IsTileBarrer(int tileX, int tileY)
{
    unsigned char type = GetTileBarrerCode(tileX, tileY);
    return (type == 0x80 || type == 0xA0);
}

bool Map::IsTileBarrerTrans(int tileX, int tileY)
{
    unsigned char type = GetTileBarrerCode(tileX, tileY);
    return (type == 0x40 || type == 0x60);
}

bool Map::GetTilePosition(int pixelX, int pixelY, int *tileX, int *tileY)
{
    if(pixelX > mPixelWidth ||
            pixelY > mPixelHeight) return false;

    return GetTilePosition(tileX, tileY, pixelX, pixelY);
}
bool Map::GetTilePosition(int *tileX, int *tileY,int pixelX, int pixelY)
{
    if(tileX == NULL ||
            tileY == NULL ||
            pixelX < 0 ||
            pixelY < 0 ) return false;

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

    return true;
}

bool Map::GetPixelPosition(int Column, int Row, int *pixelX, int *pixelY)
{
    if(Column > mCol ||
            Row > mRow) return false;

	return GetPixelPosition(pixelX, pixelY, Column, Row);
}

bool Map::GetPixelPosition(int* pixelX, int* pixelY, int Column, int Row)
{
    if(pixelX == NULL ||
            pixelY == NULL ||
            Column < 0 ||
            Row < 0 ) return false;

    long basex, basey;
    basex = (Row%2) * 32 + 64 * Column;
    basey = 16 * Row;

    *pixelX = basex;
    *pixelY = basey;

    return true;
}

bool Map::GetTileCenterPixelPosition(int Column, int Row, int* pixelX, int* pixelY)
{
    if(GetPixelPosition(Column, Row, pixelX, pixelY))
    {
        *pixelX += 32;
        *pixelY += 16;
        return true;
    }
    return false;
}

bool Map::GetTileCenterPixelPosition(int* pixelX, int* pixelY, int Column, int Row)
{
	if(GetPixelPosition(pixelX, pixelY,Column, Row))
    {
        *pixelX += 32;
        *pixelY += 16;
        return true;
    }
    return false;
}

void Map::DrawTile(long Column, long Row,
                   long TileWidth, long TileHeight,
                   unsigned char* TileData, wxImage *img)
{
    if(TileData == NULL || Column > mCol || Row > mRow || img == NULL) return;

    //tile top-left position
    long basex, basey;
    bool hasAlpha = img->HasAlpha();
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
                int iX = graphx + wi;
            	int iY = graphy + hi;
            	if(iX >= 0 && iY >= 0 && iX < img->GetWidth() && iY < img->GetHeight())
				{
					img->SetRGB(wxRect(iX, iY, 1, 1),
                            TileData[datai],
                            TileData[datai + 1],
                            TileData[datai + 2]);
					if(hasAlpha)img->SetAlpha(iX, iY, 0xFF);
				}
            }

            datai += 4;
        }
    }
}

void Map::DrawTmxTile(long Column, long Row,const Proto::TmxMap::TileImg* tileImg, wxImage *img)
{
	if(Column > mCol || Row > mRow || tileImg == NULL || img == NULL) return;

	//tile top-left position
    long basex, basey;
    bool hasAlpha = img->HasAlpha();
    basex = (Row%2) * 32 + 64 * Column;
    basey = 16 * Row;

    long x = tileImg->region().x();
    long y = tileImg->region().y();
    long w = tileImg->region().w();
    long h = tileImg->region().h();

    //graph top-left position
    long graphx, graphy;
    graphx = basex + tileImg->offset().x();
    graphy = basey + (32 - h) + tileImg->offset().y();

    ImgData *imgData = &(mTmxImgSrc[tileImg->imgindex()]);

    if(imgData->data != nullptr)
	{
		for(long hi = y; hi < y+h; hi++)
		{
			for(long wi = x; wi < x+w; wi++)
			{
				long index = (wi + hi * imgData->width)*4;
				if(imgData->data[index+3] != 0)
				{
					long imgX = graphx + wi - x;
					long imgY = graphy + hi - y;
					if(imgX >= 0 && imgY >= 0 && imgX < img->GetWidth() && imgY < img->GetHeight())
					{
						img->SetRGB(wxRect(imgX, imgY, 1, 1),
									imgData->data[index],
									imgData->data[index + 1],
									imgData->data[index + 2]);
						if(hasAlpha)img->SetAlpha(imgX, imgY, 0xFF);
					}
				}
			}
		}
	}
}


wxImage* Map::getImage(unsigned char layer, bool hasAlpha)
{
    wxImage *img = new wxImage;
    img->Create(mPixelWidth, mPixelHeight, true);
    if(hasAlpha)img->SetAlpha();

    if(layer & LAYER1) DoDrawLayer(1, img);
    if(layer & LAYER2) DoDrawLayer(2, img);
    if(layer & LAYER3) DoDrawLayer(3, img);
    if(layer & TRAP)   DrawInfoLayer(2, img);
    if(layer & BARRER) DrawInfoLayer(1, img);

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
        mTrap[i].Free();
    }

    for(int j = 0; j < 4; j++)
    {
        mBarrer[j].Free();
    }
}

void Map::AssignImgData(ImgData *imgdata, wxImage &img)
{
    if(imgdata == NULL) return;
	imgdata->Free();
    if(img.IsOk())
    {
        unsigned char *pixdata = nullptr, *alphadata = nullptr;
        int width, height;
        long datasize;

        if(img.HasAlpha()) alphadata = img.GetAlpha();
        width = img.GetWidth();
        height = img.GetHeight();
        pixdata = img.GetData();
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
            (*imgdata).data[di++] = alphadata == nullptr ? 0xFF : alphadata[ai++];
        }
    }
}











