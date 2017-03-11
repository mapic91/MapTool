#include "TmxReaderSetting.h"

#include "wx/stdpaths.h"
#include "wx/string.h"
#include "wx/file.h"
#include "wx/filename.h"

unsigned short TmxReaderSetting::Port = 26543;

namespace
{
	wxString GetSettingFilePath()
	{
		wxString path;
		path = wxStandardPaths::Get().GetExecutablePath();
		path = wxFileName::FileName(path).GetPath(wxPATH_GET_VOLUME) + wxT("/resource/TmxReadHelper/PortSetting");
		return path;
	}
}

unsigned short TmxReaderSetting::GetPort()
{
	return Port;
}

void TmxReaderSetting::SetPortValue(unsigned short port)
{
	Port = port;
}

void TmxReaderSetting::LoadConfing()
{
	wxString path = GetSettingFilePath();
    if(wxFile::Exists(path))
    {
		long port = (long)Port;
		wxFile file;
		file.Open(path);
		if(file.IsOpened())
		{
			wxString txt;
			file.ReadAll(&txt);
			txt.ToLong(&port);
			Port = (unsigned short)port;
		}

    }
}

void TmxReaderSetting::SaveConfig()
{
	wxString path = GetSettingFilePath();
	wxString sPort = wxString::Format(wxFormatString(wxT("%u")),Port);
	wxFile out;
	out.Create(path, true);
	if(out.IsOpened())
	{
		out.Write(sPort);
		out.Close();
	}
}
