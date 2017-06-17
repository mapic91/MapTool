#include "Utils.h"

#include "wx/stdpaths.h"
#include "wx/filename.h"


wxString Utils::GetFullPath(const wxString& path)
{
	return wxFileName::FileName(wxStandardPaths::Get().GetExecutablePath()).GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + path;
}
