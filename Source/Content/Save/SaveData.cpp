#include "SaveData.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdlib>
#include <fstream>


std::string SaveData::BuildSavePath(const std::string& folderName, const std::string& fileName)
{
	char* appData = nullptr;
	size_t length = 0;
	if (_dupenv_s(&appData, &length, "APPDATA") != 0 || appData == nullptr)
	{
		return fileName;
	}

	std::string directory = std::string(appData) + "\\" + folderName;
	free(appData);

	CreateDirectoryA(directory.c_str(), nullptr);
	return directory + "\\" + fileName;
}

void SaveData::InitSaveData(const std::string& folderName, const std::string& fileName)
{
	path = (BuildSavePath(folderName, fileName));
	isInit = true;
}

bool SaveData::Load()
{
	if (isInit == false)
	{
		return false;
	}

	singleplayerHighscore = 0;
	multiplayerHighscore = 0;

	std::ifstream in(path);
	if (in.is_open() == false)
	{
		return false;
	}

	int sp = 0, mp = 0;
	if (!(in >> sp >> mp) || sp < 0 || mp < 0)
	{
		return false;
	}

	singleplayerHighscore = sp;
	multiplayerHighscore = mp;
	return true;
}

bool SaveData::Save() const
{
	if (isInit == false)
	{
		return false;
	}

	std::string tmp = path + ".tmp";
	{
		std::ofstream out(tmp, std::ios::trunc);
		if (out.is_open() == false)
		{
			return false;
		}
		out << singleplayerHighscore << '\n' << multiplayerHighscore << '\n';
		if (out.good() == false)
		{
			return false;
		}
	}
	return MoveFileExA(tmp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING) != 0;
}

bool SaveData::SubmitSingleplayerScore(int score)
{
	if (isInit == false)
	{
		return false;
	}

	singleplayerHighscore = score;
	Save();
	return true;
}

bool SaveData::SubmitMultiplayerScore(int score)
{
	if (isInit == false)
	{
		return false;
	}

	multiplayerHighscore = score;
	Save();
	return true;
}