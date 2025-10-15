#ifndef MPCDECODE_HPP_INCLUDED
#define MPCDECODE_HPP_INCLUDED

#include "wx/string.h"
#include "wx/image.h"
#include "wx/dcclient.h"

#include "mydefs.hpp"
#include "ImageDecode.hpp"

#include <list>

class MpcDecode : public ImageDecode
{
public:
    enum COLOUR_MODLE {PIC_RGB,PIC_RGBA, PIC_BGRA};
    MpcDecode();
    ~MpcDecode();
    MpcDecode(const wxString MpcFilePath);
    void SetFilePath(const wxString MpcFilePath) override;
    wxString GetFilePath() override;
    bool ReadFile() override;
    bool ReadFile(const wxString MpcFilePath) override;
    wxString GetVersionInfo() override;
    unsigned long GetFramesDataLengthSum();
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
    long GetLeft() override { return 0; }

    //data must be free() if won't use anymore
    Palette_Colour GetTransparentColor();
    unsigned char* GetDecodedFrameData(const unsigned long index, long* Width = NULL, long* Height = NULL,
                                       COLOUR_MODLE mod = PIC_RGB);
    //buffered RGBA data
    unsigned char* GetBuffedFrameData(const unsigned long index, long* Width = NULL, long* Height = NULL);
    wxImage GetFrameImage(const unsigned long index);
    wxBitmap GetDirectionBitmapFromBufferdData(long direction, long index = 0) override;

    void BufferData() override;

    Palette PaletteData;


private:
    void Init();

    void FreeBufferData();

    Frame_Data *first;
    std::list<wxBitmap*> BufferdFrame;
    MpcFileHead FileHead;
    wxString FilePath;
    unsigned long FrameDataBegPos;

};

#endif // MPCDECODE_HPP_INCLUDED
