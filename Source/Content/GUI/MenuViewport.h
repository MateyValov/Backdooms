#pragma once

#include "Architecture/Viewport.h"

class MainMenu;
class ScoreWidget;
class PauseWidget;

class MenuViewport : public Viewport
{
private:
	MainMenu* mainMenu = nullptr;
	ScoreWidget* scoreWidget = nullptr;
	PauseWidget* pauseWidget = nullptr;

public:
	MenuViewport(int drawOrder, Screenspace screenspace);

	void ShowMainMenu(bool value);
	void ShowScoreWidget(bool value);
	void ShowPauseWidget(bool value);

	ScoreWidget* GetScoreWidget();
};
