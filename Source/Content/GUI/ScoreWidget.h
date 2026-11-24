#pragma once

#include "Architecture/GUI/Widget.h"

class GUI_Text;

class ScoreWidget : public Widget
{
private:
	GUI_Text* currentScore;
	GUI_Text* highScore;

public:
	ScoreWidget();

	void SetScore(int score);
	void SetHighscore(int highscore);
};
