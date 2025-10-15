#include "AsfDecode.hpp"
#include "wx/image.h"
#include "wx/msgdlg.h"

#include <cstring>
#include <fstream>

AsfDecode::AsfDecode()
{
    Init();
}
AsfDecode::~AsfDecode()
{
    FreeBufferData();
}
AsfDecode::AsfDecode(const wxString AsfFilePath)
{
    Init();
    FilePath = AsfFilePath;
}
void AsfDecode::Init()
{
    memset(&FileHead,0, sizeof(AsfFileHead));
    memset(&PaletteData, 0, sizeof(PaletteData));
}
void AsfDecode::SetFilePath(const wxString AsfFilePath)
{
    FilePath = AsfFilePath;
}
wxString AsfDecode::GetFilePath()
{
    return FilePath;
}
bool AsfDecode::ReadFile()
{
    FreeBufferData();
    Init();
    if(FilePath.IsEmpty()) return false;
    else
    {
        unsigned char temp4b[4],temp;
        unsigned long tempul =0;
        std::ifstream asffile(FilePath.char_str(), std::ios_base::binary|std::ios_base::in);
        if(!asffile.is_open()) return false;

        //Read head
        asffile.read(FileHead.VersionInfo, 16);
        if(asffile.fail() ||
           strncmp(FileHead.VersionInfo, "ASF", sizeof("ASF")-1) != 0)
        {
            return false;
        }
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        if(tempul > 0x7FFFFFFF) tempul = (unsigned long)(-(long)tempul);
        FileHead.GlobleWidth = (long)(0x0 | tempul);
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        if(tempul > 0x7FFFFFFF) tempul = (unsigned long)(-(long)tempul);
        FileHead.GlobleHeight = (long)(0x0 | tempul);
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.FrameCounts = tempul;
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Direction = (long)(0x0 | tempul);
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.ColourCounts = tempul;
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Interval = (long)(0x0 | tempul);
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Bottom = (long)(0x0 | tempul);
        asffile.read((char*)temp4b, 4);
        tempul = 0;
        tempul |= ( ((unsigned long)temp4b[0] & (unsigned long)0xFF) );
        tempul |= ( ((unsigned long)temp4b[1] & (unsigned long)0xFF) << 8 );
        tempul |= ( ((unsigned long)temp4b[2] & (unsigned long)0xFF) << 16 );
        tempul |= ( ((unsigned long)temp4b[3] & (unsigned long)0xFF) << 24 );
        FileHead.Left = (long)(0x0 | tempul);
        asffile.read(FileHead.Reserved, 16);

        PaletteData.Size = FileHead.ColourCounts;

        for(unsigned long i = 0; i < FileHead.ColourCounts; i++)
        {
            asffile.read((char*)&temp, 1);
            PaletteData.Data[i].Blue = temp;
            asffile.read((char*)&temp, 1);
            PaletteData.Data[i].Green = temp;
            asffile.read((char*)&temp, 1);
            PaletteData.Data[i].Red = temp;
            asffile.read((char*)&temp, 1);
            PaletteData.Data[i].Alpha = temp;
        }

        FrameDataBegPos = asffile.tellg();
        if(!asffile.good())return false;
        asffile.close();
    }

    return true;
}
bool AsfDecode::ReadFile(const wxString AsfFilePath)
{
    FilePath = AsfFilePath;
    return ReadFile();
}
wxString AsfDecode::GetVersionInfo()
{
    return wxString(FileHead.VersionInfo, wxConvLibc);
}
unsigned long AsfDecode::GetFramesCounts()
{
    return FileHead.FrameCounts;
}
long AsfDecode::GetGlobleWidth()
{
    return FileHead.GlobleWidth;
}
long AsfDecode::GetGlobleHeight()
{
    return FileHead.GlobleHeight;
}
long AsfDecode::GetDirection()
{
    return FileHead.Direction;
}
unsigned long AsfDecode::GetColourCounts()
{
    return FileHead.ColourCounts;
}
unsigned long AsfDecode::GetInterval()
{
    return FileHead.Interval;
}
long AsfDecode::GetBottom()
{
    return FileHead.Left;//I make a mistake,so...
}
long AsfDecode::GetLeft()
{
    return FileHead.Bottom;//I make a mistake,so...
}
unsigned char* AsfDecode::GetDecodedFrameData(unsigned long index, long* Width, long* Height,
        COLOUR_MODLE mod, bool *isTransparent, Palette_Colour *TransparentColor, unsigned char transmask)
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
        Palette_Colour transcol;
        if(TransparentColor != NULL)
        {
            transcol.Red = TransparentColor->Red;
            transcol.Green = TransparentColor->Green;
            transcol.Blue = TransparentColor->Blue;
            //transcol.Alpha =  TransparentColor->Alpha;
        }
        else
        {
            transcol.Red = 0;
            transcol.Green = 0;
            transcol.Blue = 0;
        }

        unsigned long temppos = 0, datalength, width = FileHead.GlobleWidth,
                      height = FileHead.GlobleHeight, curdecposition = 0;
        unsigned char temp[8], alpha;
        std::ifstream asffile(FilePath.char_str(), std::ios_base::binary|std::ios_base::in);
        long decdatalen;
        unsigned char* decdata;
        if(!asffile.is_open())
        {
            return NULL;
        }
        asffile.seekg(FrameDataBegPos+index*8);
        asffile.read((char*)temp, 8);
        temppos = 0;
        temppos |= ( ((unsigned long)temp[0] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[1] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[2] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[3] & (unsigned long)0xFF) << 24 );
        asffile.seekg(temppos);
        if(asffile.eof())
        {
            asffile.close();
            return NULL;
        }
        temppos = 0;
        temppos |= ( ((unsigned long)temp[4] & (unsigned long)0xFF) );
        temppos |= ( ((unsigned long)temp[5] & (unsigned long)0xFF) << 8 );
        temppos |= ( ((unsigned long)temp[6] & (unsigned long)0xFF) << 16 );
        temppos |= ( ((unsigned long)temp[7] & (unsigned long)0xFF) << 24 );
        datalength = temppos;

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
            asffile.close();
            return NULL;
        }

        for(long datidx = 0; datidx < decdatalen;)
        {
            switch(mod)
            {
            case PIC_RGB:
                decdata[datidx++] = transcol.Red;
                decdata[datidx++] = transcol.Green;
                decdata[datidx++] = transcol.Blue;
                break;
            case PIC_RGBA:
                decdata[datidx++] = transcol.Red;
                decdata[datidx++] = transcol.Green;
                decdata[datidx++] = transcol.Blue;
                decdata[datidx++] = 0x00;
                break;
            case PIC_BGRA:
                decdata[datidx++] = transcol.Blue;
                decdata[datidx++] = transcol.Green;
                decdata[datidx++] = transcol.Red;
                decdata[datidx++] = 0x00;
                break;
            default:
                break;
            }
        }


        if(isTransparent != NULL)
        {
            *isTransparent = true;//always transparent
        }
        asffile.read((char*)temp,2);
        for(unsigned long i = 0; i < datalength - 20;)
        {
            if(curdecposition > (unsigned long)decdatalen)
            {
                free(decdata);
                asffile.close();
                return NULL;
            }
            if(temp[1] == (unsigned char)0x00)
            {
                temppos = temp[0];
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
                        decdata[curdecposition++] = 0x0;
                        break;
                    case PIC_BGRA:
                        decdata[curdecposition++] = transcol.Blue;
                        decdata[curdecposition++] = transcol.Green;
                        decdata[curdecposition++] = transcol.Red;
                        decdata[curdecposition++] = 0x0;
                        break;
                    default:
                        //can't be here
                        return NULL;
                    }
                }
            }
            else
            {
                temppos = temp[0];
                alpha = temp[1];
                for(unsigned char m = 0; m < temppos; m++)
                {
                    if(asffile.fail())
                    {
                        asffile.close();
                        free(decdata);
                        return NULL;
                    }
                    asffile.read((char*)temp,1);
                    i++;
                    switch(mod)
                    {
                    case PIC_RGB:
                        if(alpha < transmask)
                        {
                             decdata[curdecposition++] = transcol.Red;
                             decdata[curdecposition++] = transcol.Green;
                             decdata[curdecposition++] = transcol.Blue;
                        }
                        else
                        {
                            decdata[curdecposition++] = (unsigned char)(((double)PaletteData.Data[temp[0]].Red)*((double)alpha/(double)0xFF) +
                            ((double)transcol.Red)*((double)(0xFF-alpha)/(double)0xFF));
                            decdata[curdecposition++] = (unsigned char)(((double)PaletteData.Data[temp[0]].Green)*((double)alpha/(double)0xFF) +
                            ((double)transcol.Green)*((double)(0xFF-alpha)/(double)0xFF));
                            decdata[curdecposition++] = (unsigned char)(((double)PaletteData.Data[temp[0]].Blue)*((double)alpha/(double)0xFF) +
                            ((double)transcol.Blue)*((double)(0xFF-alpha)/(double)0xFF));
                        }
                        break;
                    case PIC_RGBA:
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Red;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Green;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Blue;
                        decdata[curdecposition++] = alpha;
                        break;
                    case PIC_BGRA:
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Blue;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Green;
                        decdata[curdecposition++] = PaletteData.Data[temp[0]].Red;
                        decdata[curdecposition++] = alpha;
                        break;
                    default:
                        //can't be here
                        return NULL;
                    }
                }
            }
            asffile.read((char*)temp,2);
            i += 2;
        }

        asffile.close();
        return decdata;
    }
    else return NULL;
}

wxBitmap AsfDecode::GetDirectionBitmapFromBufferdData(long direction, long indexOff)
{
	if(direction < 0) direction = 0;
	if(indexOff < 0) indexOff = 0;
    if(direction >= GetDirection() ||
       GetFramesCounts() == 0 ||
       GetDirection() == 0) return wxNullBitmap;
	int index;
	if(GetDirection() < 1)
	{
		index = 0;
	}
	else
	{
		index = direction * GetFramesCounts() / GetDirection();
	}
	index += indexOff;
	if(index < 0 || index >= (int)GetFramesCounts()) return wxNullBitmap;
	std::list<wxBitmap*>::iterator it = BufferdFrame.begin();
	for(int i = 0; i < index; i++, it++);
    return (*(*it));
}

void AsfDecode::BufferData()
{
    FreeBufferData();
    long width, height;
    unsigned char* data;
    if(GetFramesCounts() == 0) return;
    for(unsigned long i = 0; i < GetFramesCounts(); i++)
    {
        data = GetDecodedFrameData(i, &width, &height, PIC_RGBA);
		wxImage image = GetImageFromRgbaData(data, (int)width, (int)height);
		free(data);
		wxBitmap *bmp = new wxBitmap(image);
		BufferdFrame.push_back(bmp);
    }
}

void AsfDecode::FreeBufferData()
{
    for(std::list<wxBitmap*>::iterator it = BufferdFrame.begin();
		it != BufferdFrame.end();
		it++)
	{
		delete (*it);
	}
	BufferdFrame.clear();
}
