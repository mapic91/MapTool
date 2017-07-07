#include "LoadXPM.h"
#include <memory>

#include "ICO/L.h"
#include "ICO/L_Disabled.h"

wxBitmap LoadXPM(const wxString& name)
{
	std::unique_ptr<wxImage> img;
	if (name == wxT("L"))
	{
		img.reset(new wxImage(L_xpm));
	}
	else if(name == wxT("L_Disabled"))
	{
		img.reset(new wxImage(L_Disabled_xpm));
	}

	if(img != nullptr)
	{
		img->SetMaskColour(0xFF, 0xFF, 0xFF);
		img->SetMask(true);
		return wxBitmap(*img.get());
	}

	return wxBitmap();
}
