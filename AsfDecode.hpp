#ifndef ASFDECODE_HPP_INCLUDED
#define ASFDECODE_HPP_INCLUDED

#include "wx/string.h"
#include "wx/image.h"

#include "mydefs.hpp"

class AsfDecode
{
public:
    enum COLOUR_MODLE{PIC_RGB,PIC_RGBA, PIC_BGRA};
    AsfDecode();
    ~AsfDecode();
    void Init();
    AsfDecode(const wxString AsfFilePath);
    void SetAsfFile(const wxString AsfFilePath);
    wxString GetFilePath();
    bool ReadAsfFile();
    bool ReadAsfFile(const wxString AsfFilePath);
    wxString GetVersionInfo();
    unsigned long GetFramesCounts();
    //unsigned long GetFrameDataLength(const unsigned long index);
    long GetGlobleWidth();
    long GetGlobleHeight();
    //long GetFrameWidth(const unsigned long index);
    //long GetFrameHeight(const unsigned long index);
    long GetDirection();
    unsigned long GetColourCounts();
    unsigned long GetInterval();
    long GetBottom();
    long GetLeft();

    //data must be free() if won't use anymore
    unsigned char* GetDecodedFrameData(unsigned long index, long* Width = NULL, long* Height = NULL,
                                       COLOUR_MODLE mod = PIC_RGB,bool *isTransparent = NULL,
                                       Palette_Colour *TransparentColor = NULL, unsigned char transmask = 0);
    unsigned char* GetBuffedFrameData(unsigned long index, long* Width = NULL, long* Height = NULL);
    wxImage GetImageFromBuffedData(unsigned long index);
    wxImage GetDirectionImageFromBufferdData(unsigned long direction);

    void BufferData();

    Palette PaletteData;

private:
    void FreeBufferData();

    Frame_Data *first;
    AsfFileHead FileHead;
    wxString FilePath;
    unsigned long FrameDataBegPos;

};

#endif // ASFDECODE_HPP_INCLUDED
