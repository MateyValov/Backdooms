#pragma once

#include "Architecture/Viewport.h"

class MainMenu;
class ScoreWidget;

class MenuViewport : public Viewport
{
private:
	MainMenu* mainMenu = nullptr;
	ScoreWidget* scoreWidget = nullptr;
	

public:
	MenuViewport(Screenspace screenspace);

	void ShowMainMenu(bool value);
	void ShowScoreWidget(bool value);

};
