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
	public:
		int MapToolX = -1;
		int MapToolY = -1;
		int MapToolWidth = -1;
		int MapToolHeight = -1;

		int NpcDialogX = -1;
		int NpcDialogY = -1;
		int NpcDialogWidth = -1;
		int NpcDialogHeight = -1;

		int ObjDialogX = -1;
		int ObjDialogY = -1;
		int ObjDialogWidth = -1;
		int ObjDialogHeight = -1;

		int NpcDetailDescBoxHeight = -1;
		int NpcDetailSpliterPos = -1;

		int ObjDetailDescBoxHeight = -1;
		int ObjDetailSpliterPos = -1;
};

#endif // SETTINGS_H
