#pragma once

#include <string>

class SaveData
{
private:
	bool isInit = false;
	int singleplayerHighscore = 0;
	int multiplayerHighscore = 0;
	std::string path;

	std::string BuildSavePath(const std::string& folderName, const std::string& fileName);

public:
	void InitSaveData(const std::string& folderName, const std::string& fileName);

	bool Load();
	bool Save() const;

	int GetSingleplayerHighscore() const { return singleplayerHighscore; }
	int GetMultiplayerHighscore() const { return multiplayerHighscore; }

	bool SubmitSingleplayerScore(int score);
	bool SubmitMultiplayerScore(int score);
};
