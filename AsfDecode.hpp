#ifndef ASFDECODE_HPP_INCLUDED
#define ASFDECODE_HPP_INCLUDED

#include "wx/string.h"
#include "wx/image.h"
#include "wx/dcclient.h"

#include "mydefs.hpp"
#include "ImageDecode.hpp"

#include <list>

class AsfDecode : public ImageDecode
{
public:
    enum COLOUR_MODLE{PIC_RGB,PIC_RGBA, PIC_BGRA};
    AsfDecode();
    ~AsfDecode();
    AsfDecode(const wxString AsfFilePath);
    void SetFilePath(const wxString AsfFilePath) override;
    wxString GetFilePath() override;
    bool ReadFile() override;
    bool ReadFile(const wxString AsfFilePath) override;
    wxString GetVersionInfo() override;
    unsigned long GetFramesCounts() override;
    //unsigned long GetFrameDataLength(const unsigned long index);
    long GetGlobleWidth() override;
    long GetGlobleHeight() override;
    //long GetFrameWidth(const unsigned long index);
    //long GetFrameHeight(const unsigned long index);
    long GetDirection() override;
    unsigned long GetColourCounts() override;
    unsigned long GetInterval() override;
    long GetBottom() override;
    long GetLeft() override;

    //data must be free() if won't use anymore
    unsigned char* GetDecodedFrameData(unsigned long index, long* Width = NULL, long* Height = NULL,
                                       COLOUR_MODLE mod = PIC_RGB,bool *isTransparent = NULL,
                                       Palette_Colour *TransparentColor = NULL, unsigned char transmask = 0);
    wxBitmap GetDirectionBitmapFromBufferdData(long direction, long index = 0) override;

    void BufferData() override;

    Palette PaletteData;

private:
	void Init();
    void FreeBufferData();

    //Frame_Data *first;
    std::list<wxBitmap*> BufferdFrame;
    AsfFileHead FileHead;
    wxString FilePath;
    unsigned long FrameDataBegPos;

};

#endif // ASFDECODE_HPP_INCLUDED
