#include "Settings.h"

#include <fstream>

const char* SettingFileName = "MapTool.setting";

Settings::Settings()
{
	m_fpsMilliSeconds = 100;
	LoadSetting();
}

Settings::~Settings()
{
	//dtor
}

Settings Settings::TheSetting = Settings();

void Settings::LoadSetting()
{
	std::ifstream file(SettingFileName);
	if(file.is_open())
	{
		file >> m_fpsMilliSeconds;

		if(!file.eof())
		{
			file >> MapToolX;
			file >> MapToolY;
			file >> MapToolWidth;
			file >> MapToolHeight;

			file >> NpcDialogX;
			file >> NpcDialogY;
			file >> NpcDialogWidth;
			file >> NpcDialogHeight;

			file >> ObjDialogX;
			file >> ObjDialogY;
			file >> ObjDialogWidth;
			file >> ObjDialogHeight;
		}

		if(!file.eof())
		{
			file >> NpcDetailDescBoxHeight;
			file >> NpcDetailSpliterPos;

			file >> ObjDetailDescBoxHeight;
			file >> ObjDetailSpliterPos;
		}
	}
}

void Settings::SaveSetting()
{
	std::ofstream file(SettingFileName);
	if(file.is_open())
	{
		file << m_fpsMilliSeconds << "\n";

		file << MapToolX << "\n";
		file << MapToolY << "\n";
		file << MapToolWidth << "\n";
		file << MapToolHeight << "\n";

		file << NpcDialogX << "\n";
		file << NpcDialogY << "\n";
		file << NpcDialogWidth << "\n";
		file << NpcDialogHeight << "\n";

		file << ObjDialogX << "\n";
		file << ObjDialogY << "\n";
		file << ObjDialogWidth << "\n";
		file << ObjDialogHeight << "\n";

		file << NpcDetailDescBoxHeight << "\n";
		file << NpcDetailSpliterPos << "\n";

		file << ObjDetailDescBoxHeight << "\n";
		file << ObjDetailSpliterPos << "\n";
	}
}

int Settings::GetFpsMilliseconds()
{
	return m_fpsMilliSeconds;
}

void Settings::SetFpsMilliseconds(int milliSeconds)
{
	m_fpsMilliSeconds = milliSeconds;
	SaveSetting();
}
