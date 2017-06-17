#include "TmxReaderClient.h"
#include "TmxReaderSetting.h"

#include "wx/msgdlg.h"
#include "wx/filename.h"

#include <string>

TmxReaderClient::TmxReaderClient()
	: mIsExited(false)
{
	mClient.SetFlags(wxSOCKET_WAITALL);
	m_port = TmxReaderSetting::GetPort();
}

TmxReaderClient::~TmxReaderClient()
{
	//dtor
}
Proto::TmxMap* TmxReaderClient::GetTmxMap(const wxString& mapPath)
{
	Connect();
	if(mClient.IsConnected())
	{
		wxFileName fileName(mapPath);
		if(fileName.MakeAbsolute())
		{
			wxString fullPath = fileName.GetFullPath();
			const wxScopedCharBuffer& pathData = fullPath.utf8_str();
			if(Send(OpenMapCode, pathData.data(), pathData.length()))
			{
				std::vector<char> data = Read();
				if(data.size() > 0 && data[0] == ResultOk)
				{
					std::string strdata(++data.begin(), data.end());
					mTmxMap.Clear();
					mTmxMap.ParseFromString(strdata);
					return &mTmxMap;
				}
				else
				{
					wxMessageBox(wxT("Tmx地图读取失败！"), wxT("消息"), wxCENTER| wxICON_ERROR);
				}
			}

		}
	}
	return nullptr;
}

void TmxReaderClient::Connect(bool silent)
{
	if(mClient.IsConnected()) return;

	wxIPaddress * addr;
	wxIPV4address addrIp4;
	addr = &addrIp4;
	addr->Hostname("localhost");
	addr->Service(m_port);

	mClient.Connect(*addr);

	if(!mClient.IsConnected() && !silent)
	{
		wxMessageBox(wxT("Tmx地图读取服务连接失败！"), wxT("消息"), wxCENTER| wxICON_ERROR);
	}
}

bool TmxReaderClient::IsConnected()
{
	return mClient.IsConnected();
}


void TmxReaderClient::Exit()
{
	if(mIsExited) return;
	Connect();
	if(mClient.IsConnected())
	{
		Send(ExitServerCode, nullptr, 0);
		mClient.Close();
	}
	mIsExited = true;
}

bool TmxReaderClient::Send(char code, const char* data, int length)
{
	int size = 4 + 1 + length;
	char* buffer = new char[size];
	IntToCharLE(size - 4, buffer, 0);
	buffer[4] = code;
	if(length > 0) memcpy(buffer+5, data, length);
	mClient.Write(buffer, size);
	delete[] buffer;
	if(mClient.Error())
	{
		wxMessageBox(wxT("与Tmx地图读取服务数据传输出错！"), wxT("消息"), wxCENTER| wxICON_ERROR);
		return false;
	}
	return true;
}

std::vector<char> TmxReaderClient::Read()
{
	const size_t bufferSize = 1024;
	size_t bufferReaed = 0;
	size_t dataReaded = 0;
	char* buffer = new char[bufferSize];
	std::vector<char> data;
	while(true)
	{
		mClient.Read(buffer + bufferReaed,bufferSize - bufferReaed);
		size_t readed = mClient.LastReadCount();
		if(readed > 0)
		{
			bufferReaed += readed;
			if(data.size() == 0)
			{
				if(readed >= 4)
				{
					int length = CharToIntLE((unsigned char*)buffer);
					data.resize(length);
					memcpy(data.data(), buffer+4, bufferReaed-4);
					dataReaded+=(bufferReaed-4);
					bufferReaed = 0;
				}
			}
			else
			{
				if(data.size() >= dataReaded+bufferReaed)
				{
					memcpy(data.data()+dataReaded, buffer, bufferReaed);
					dataReaded+=bufferReaed;
					bufferReaed = 0;
				}

				if(data.size() == dataReaded)
				{
					break;
				}
			}
		}

		if(mClient.Error() && data.size() != dataReaded)
		{
			data.resize(0);
		}
	}

	delete[] buffer;
	return data;
}

void TmxReaderClient::IntToCharLE(int value, char* buffer, int offset)
{
	buffer[offset] = (char)(value & 0xFF);
	buffer[offset+1] = (char)((value >> 8) & 0xFF);
	buffer[offset+2] = (char)((value >> 16) & 0xFF);
	buffer[offset+3] = (char)((value >> 24) & 0xFF);
}

int TmxReaderClient::CharToIntLE(unsigned char* in)
{
	int temp = 0;
            temp |= (unsigned int)in[0];
            temp |= (((unsigned int)in[1]) << 8);
            temp |= (((unsigned int)in[2]) << 16);
            temp |= (((unsigned int)in[3]) << 24);
            return temp;
}
