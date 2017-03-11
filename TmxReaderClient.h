#ifndef TMXREADERCLIENT_H
#define TMXREADERCLIENT_H

#include "wx/socket.h"
#include "wx/string.h"

#include "TmxMapMessage.pb.h"

#include <vector>

class TmxReaderClient
{
	public:

		TmxReaderClient();
		virtual ~TmxReaderClient();

		Proto::TmxMap* GetTmxMap(const wxString& mapPath);
		void Connect(bool silent = false);
		bool IsConnected();
		void Exit();

		bool Send(char code, const char* data, int length);
		std::vector<char> Read();

		static void IntToCharLE(int value, char *buffer, int offset = 0);
		static int CharToIntLE(unsigned char *in);

	private:
		unsigned short m_port = 26543;

		char const ExitServerCode = 0;
		char const OpenMapCode = 1;

		char const ResultOk = 0;
		char const ResultFailed = 1;

		bool mIsExited;

		wxSocketClient mClient;
		Proto::TmxMap mTmxMap;
};

#endif // TMXREADERCLIENT_H
