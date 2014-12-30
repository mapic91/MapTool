#include "Settings.h"

#include <fstream>

const char* SettingFileName = "MapTool.setting";

Settings::Settings()
{
	m_fpsMilliSeconds = 20;
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
	}
}

void Settings::SaveSetting()
{
	std::ofstream file(SettingFileName);
	if(file.is_open())
	{
		file << m_fpsMilliSeconds;
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
