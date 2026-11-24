#include "MenuViewport.h"

#include "Content/GUI/MainMenu.h"
#include "Content/GUI/ScoreWidget.h"

MenuViewport::MenuViewport(Screenspace screenspace) : Viewport(screenspace)
{
	mainMenu = new MainMenu();
	scoreWidget = new ScoreWidget();

	elements.push_back(mainMenu);
	elements.push_back(scoreWidget);

	ShowMainMenu(true);
}

void MenuViewport::ShowMainMenu(bool value)
{
	mainMenu->SetActive(value);
}

void MenuViewport::ShowScoreWidget(bool value)
{
	scoreWidget->SetActive(value);
}
