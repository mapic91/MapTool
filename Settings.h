#ifndef SETTINGS_H
#define SETTINGS_H


class Settings
{
	public:
		Settings();
		virtual ~Settings();

	public:
		void LoadSetting();
		void SaveSetting();
		int GetFpsMilliseconds();
		void SetFpsMilliseconds(int milliSeconds);

	public:
		static Settings TheSetting;
	protected:
	private:
		int m_fpsMilliSeconds;
};

#endif // SETTINGS_H
