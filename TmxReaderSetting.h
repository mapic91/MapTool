#ifndef TMXREADERSETTING_H
#define TMXREADERSETTING_H


class TmxReaderSetting
{
	public:
		static unsigned short Port;

		static unsigned short GetPort();
		static void SetPortValue(unsigned short port);

		static void LoadConfing();
		static void SaveConfig();
};

#endif // TMXREADERSETTING_H
