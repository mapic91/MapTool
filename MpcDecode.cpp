#include "MpcDecode.hpp"
#include "wx/image.h"
#include "wx/msgdlg.h"
#include "wx/filename.h"

#include <cstring>
#include <fstream>

MpcDecode::MpcDecode()
{
    Init();
}
MpcDecode::~MpcDecode()
{
    FreeBufferData();
}
MpcDecode::MpcDecode(const wxString MpcFilePath)
{
    Init();
    FilePath = MpcFilePath;
}
void MpcDecode::Init()
{
    memset(&FileHead,0, sizeof(MpcFileHead));
    memset(&PaletteData, 0, sizeof(PaletteData));
    first = NULL;
}
void MpcDecode::SetMpcFile(const wxString MpcFilePath)
{
    FilePath = MpcFilePath;
}
wxString MpcDecode::GetFilePath()
{
    return FilePath;
}
bool MpcDecode::ReadMpcFile()
{
    if(FilePath.IsEmpty()) return false;
    else
    {
        unsigned char temp4b[4],temp;
        unsigned long tempul =0;
        std::ifstream mpcfile(FilePath.char_str(), std::ios_base::binary|std::ios_base::in);
        if(!mpcfile.is_open()) return false;

        //Read head
        mpcfile.read(FileHead.VersionInfo, 64);
        if(mpcfile.fail() ||
                strncmp(FileHead.VersionInfo,
                        "MPC File Ver",
                        sizeof("MPC File Ver")-1) != 0)
        {
            return false;
        }
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.FramesDataLengthSum = tempul;
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        if(tempul > 0x7FFFFFFF) tempul = (unsigned long)(-(long)tempul);
        FileHead.GlobleWidth = (long)(0x0 | tempul);
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        if(tempul > 0x7FFFFFFF) tempul = (unsigned long)(-(long)tempul);
        FileHead.GlobleHeight = (long)(0x0 | tempul);
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.FrameCounts = tempul;
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Direction = (long)(0x0 | tempul);
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.ColourCounts = tempul;
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Interval = (long)(0x0 | tempul);
        mpcfile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Bottom = (long)(0x0 | tempul);
        mpcfile.read(FileHead.Reserved, 32);

        PaletteData.Size = FileHead.ColourCounts;

        for(unsigned long i = 0; i < FileHead.ColourCounts; i++)
        {
            mpcfile.read((char*)&temp, 1);
            PaletteData.Data[i].Blue = temp;
            mpcfile.read((char*)&temp, 1);
            PaletteData.Data[i].Green = temp;
            mpcfile.read((char*)&temp, 1);
            PaletteData.Data[i].Red = temp;
            mpcfile.read((char*)&temp, 1);
            PaletteData.Data[i].Alpha = temp;
        }

        FrameDataBegPos = mpcfile.tellg();
        if(!mpcfile.good())return false;
        mpcfile.close();
    }

    return true;
}
bool MpcDecode::ReadMpcFile(const wxString MpcFilePath)
{
    FilePath = MpcFilePath;
    return ReadMpcFile();
}
wxString MpcDecode::GetVersionInfo()
{
    return wxString(FileHead.VersionInfo, wxConvLibc);
}
unsigned long MpcDecode::GetFramesDataLengthSum()
{
    return FileHead.FramesDataLengthSum;
}
unsigned long MpcDecode::GetFramesCounts()
{
    return FileHead.FrameCounts;
}
long MpcDecode::GetGlobleWidth()
{
    return FileHead.GlobleWidth;
}
long MpcDecode::GetGlobleHeight()
{
    return FileHead.GlobleHeight;
}
long MpcDecode::GetDirection()
{
    return FileHead.Direction;
}
unsigned long MpcDecode::GetColourCounts()
{
    return FileHead.ColourCounts;
}
unsigned long MpcDecode::GetInterval()
{
    return FileHead.Interval;
}
long MpcDecode::GetBottom()
{
    return FileHead.Bottom;
}

unsigned char* MpcDecode::GetDecodedFrameData(const unsigned long index, long* Width, long* Height,
        COLOUR_MODLE mod)
{
    switch(mod)
    {
    case PIC_RGB:
    case PIC_BGRA:
    case PIC_RGBA:
        break;
    default:
        return NULL;
    }
    if(index < FileHead.FrameCounts)
    {
        unsigned long temppos = 0,
                      datalength, //frame data length
                      width, //Frame width
                      height, //Frame height
                      curdecposition = 0; //Current decode position
        unsigned char temp[8];
        std::ifstream mpcfile(FilePath.char_str(), std::ios_base::binary|std::ios_base::in);
        long decdatalen; //decoded data length
        unsigned char* decdata; //decoded data
        Palette_Colour transcol;

        transcol.Red = 0xFF;
        transcol.Green = 0xFF;
        transcol.Blue = 0xFF;
        transcol.Alpha = 0x00;

        if(!mpcfile.is_open())
        {
            return NULL;
        }
        mpcfile.seekg(FrameDataBegPos+index*4);
        mpcfile.read((char*)temp, 4);
        temppos = 0;
        temppos |= ( ((unsigned long)temp[0] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[1] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[2] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[3] & (unsigned long)0xFF) << 24 );
        mpcfile.seekg(FrameDataBegPos + FileHead.FrameCounts*4 + temppos);
        if(mpcfile.eof())
        {
            mpcfile.close();
            return NULL;
        }

        //process frame data
        mpcfile.read((char*)temp, 4);
        temppos = 0;
        temppos |= ( ((unsigned long)temp[0] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[1] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[2] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[3] & (unsigned long)0xFF) << 24 );
        datalength = temppos;
        mpcfile.read((char*)temp, 4);
        temppos = 0;
        temppos |= ( ((unsigned long)temp[0] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[1] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[2] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[3] & (unsigned long)0xFF) << 24 );
        width = temppos;
        mpcfile.read((char*)temp, 4);
        temppos = 0;
        temppos |= ( ((unsigned long)temp[0] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[1] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[2] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[3] & (unsigned long)0xFF) << 24 );
        height = temppos;
        mpcfile.seekg(8, std::ios_base::cur);//skip 8 null
        if(Width != NULL)*Width = width;
        if(Height != NULL)*Height = height;
        switch(mod)
        {
        case PIC_RGB:
            decdatalen = width * height * 3;
            break;
        case PIC_RGBA:
        case PIC_BGRA:
            decdatalen = width * height * 4;
            break;
        default:
            return NULL;
        }
        decdata = (unsigned char*)malloc((size_t)decdatalen);
        if(decdata == NULL)
        {
            mpcfile.close();
            return NULL;
        }

//        switch(mod)
//        {
//        case PIC_RGB:
//            for(long datidx = 0; datidx < (long)decdatalen;)//inital
//            {
//                decdata[datidx++] = transcol.Red;
//                decdata[datidx++] = transcol.Green;
//                decdata[datidx++] = transcol.Blue;
//            }
//            break;
//        case PIC_RGBA:
//            for(long datidx = 0; datidx < (long)decdatalen;)//inital
//            {
//                decdata[datidx++] = transcol.Red;
//                decdata[datidx++] = transcol.Green;
//                decdata[datidx++] = transcol.Blue;
//                decdata[datidx++] = transcol.Alpha;
//            }
//            break;
//        case PIC_BGRA:
//            for(long datidx = 0; datidx < (long)decdatalen;)//inital
//            {
//                decdata[datidx++] = transcol.Blue;
//                decdata[datidx++] = transcol.Green;
//                decdata[datidx++] = transcol.Red;
//                decdata[datidx++] = transcol.Alpha;
//            }
//            break;
//        default:
//            break;
//        }

        mpcfile.read((char*)temp,1);

        for(unsigned long i = 0; i < datalength - 20;)
        {
            if(curdecposition > (unsigned long)decdatalen)
            {
                free(decdata);
                mpcfile.close();
                return NULL;
            }
            if(temp[0] > (unsigned char)0x80)
            {
                temppos = temp[0] - (unsigned char)0x80;
                for(unsigned char j = 0; j < temppos; j++)
                {
                    switch(mod)
                    {
                    case PIC_RGB:
                        decdata[curdecposition++] = transcol.Red;
                        decdata[curdecposition++] = transcol.Green;
                        decdata[curdecposition++] = transcol.Blue;
                        break;
                    case PIC_RGBA:
                        decdata[curdecposition++] = transcol.Red;
                        decdata[curdecposition++] = transcol.Green;
                        decdata[curdecposition++] = transcol.Blue;
                        decdata[curdecposition++] = transcol.Alpha;
                        break;
                    case PIC_BGRA:
                        decdata[curdecposition++] = transcol.Blue;
                        decdata[curdecposition++] = transcol.Green;
                        decdata[curdecposition++] = transcol.Red;
                        decdata[curdecposition++] = transcol.Alpha;
                        break;
                    default:
                        //can't be here
                        ;
                    }
                }
            }
            else
            {
                temppos = temp[0];
                for(unsigned char m = 0; m < temppos; m++)
                {
                    if(mpcfile.fail())
                    {
                        mpcfile.close();
                        free(decdata);
                        return NULL;
                    }
                    mpcfile.read((char*)temp,1);
                    i++;
                    switch(mod)
                    {
                    case PIC_RGB:
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Red;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Green;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Blue;
                        break;
                    case PIC_RGBA:
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Red;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Green;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Blue;
                        decdata[curdecposition++] = 0xFF;
                        break;
                    case PIC_BGRA:
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Blue;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Green;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Red;
                        decdata[curdecposition++] = 0xFF;
                        break;
                    default:
                        //can't be here
                        return NULL;
                    }
                }
            }
            mpcfile.read((char*)temp,1);
            i++;
        }

        mpcfile.close();
        return decdata;
    }
    else return NULL;
}

unsigned char* MpcDecode::GetBuffedFrameData(const unsigned long index, long* Width, long* Height)
{
    if(index >= GetFramesCounts()) return NULL;

    Frame_Data *temp = first;
    for(unsigned long i = 0; i < index; i++)
    {
        if(temp == NULL) return NULL;
        temp = temp->next;
    }

    if(temp == NULL) return NULL;
    if(Width != NULL) *Width = temp->width;
    if(Height != NULL) *Height = temp->height;

    return temp->data;
}

wxImage MpcDecode::GetFrameImage(const unsigned long index)
{
    if(index < FileHead.FrameCounts)
    {
        long width, //frame width
             height, //frame height
             colpos, // decoded frame color data position
             imgpos; // wxImage color data position
        unsigned char *decdata = NULL, *imgdata = NULL;

        decdata = GetDecodedFrameData(index, &width, &height, PIC_RGBA);
        if(decdata == NULL)
        {
            if(!imgdata)
            {
                free(imgdata);
                imgdata = NULL;
            }
            return wxNullImage;
        }

        imgdata = (unsigned char*)malloc((size_t)width*height*3);
        if(!imgdata)
        {
            if(!decdata)
            {
                free(decdata);
                decdata = NULL;
            }
            return wxNullImage;
        }


        colpos = 0;
        imgpos = 0;
        for(long int sidx = 0; sidx < width*height; sidx++)
        {
            if(decdata[colpos+3] == 0)//fill transparent color
            {
                imgdata[imgpos] = 0xFF;
                imgdata[imgpos+1] = 0xFF;
                imgdata[imgpos+2] = 0xFF;
            }
            else//fill color
            {
                imgdata[imgpos] = decdata[colpos];
                imgdata[imgpos+1] = decdata[colpos+1];
                imgdata[imgpos+2] = decdata[colpos+2];
            }
            imgpos += 3;
            colpos += 4;
        }
        free(decdata);

        wxImage img(width,height,imgdata);
        return img;
    }
    else return  wxNullImage;
}

void MpcDecode::BufferData()
{
    FreeBufferData();
    Frame_Data *temp;
    long width, height;
    if(GetFramesCounts() == 0) return;
    first = new Frame_Data;
    temp = first;
    for(unsigned long i = 0; i < GetFramesCounts(); i++)
    {
        temp->data = GetDecodedFrameData(i, &width, &height, PIC_RGBA);
        temp->width = width;
        temp->height = height;
        if(i != GetFramesCounts() - 1)
        {
            temp->next = new Frame_Data;
            temp = temp->next;
        }
        else //last one
        {
            temp->next = NULL;
        }

    }
}

void MpcDecode::FreeBufferData()
{
    Frame_Data *temp;
    while(first != NULL)
    {
        temp = first->next;
        if(first->data != NULL) free(first->data);
        delete first;
        first = temp;
    }
    first = NULL;
}
