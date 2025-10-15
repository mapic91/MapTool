#ifndef IMAGEDECODE_HPP_INCLUDED
#define IMAGEDECODE_HPP_INCLUDED

#include "wx/string.h"
#include "wx/image.h"
#include "wx/dcclient.h"

class ImageDecode
{
public:
	virtual ~ImageDecode() {}
	virtual void SetFilePath(const wxString FilePath) = 0;
	virtual wxString GetFilePath() = 0;
    virtual bool ReadFile() = 0;
    virtual bool ReadFile(const wxString FilePath) = 0;
    virtual wxString GetVersionInfo() = 0;
    virtual unsigned long GetFramesCounts() = 0;
    virtual long GetGlobleWidth() = 0;
    virtual long GetGlobleHeight() = 0;
    virtual long GetDirection() = 0;
    virtual unsigned long GetColourCounts() = 0;
    virtual unsigned long GetInterval() = 0;
    virtual long GetBottom() = 0;
    virtual long GetLeft() = 0;
	virtual wxBitmap GetDirectionBitmapFromBufferdData(long direction, long index = 0) = 0;
    virtual void BufferData() = 0;

    wxImage GetImageFromRgbaData(unsigned char *data, int width, int height)
    {
		if(data == NULL) return wxNullImage;

		wxImage img;
		long datapos = 0;
		img.Create(width, height, true);
		img.SetAlpha();

		for(long hi = 0; hi < height; hi++)
		{
			for(long wi = 0; wi < width; wi++)
			{
				img.SetRGB(wi,
						   hi,
						   data[datapos],
						   data[datapos + 1],
						   data[datapos + 2]);
				img.SetAlpha(wi, hi, data[datapos + 3]);
				datapos += 4;
			}
		}

		return img;
    }
};

#endif // IMAGEDECODE_HPP_INCLUDED
