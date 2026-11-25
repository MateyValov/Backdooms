#include "MenuViewport.h"

#include "Content/GUI/MainMenu.h"
#include "Content/GUI/ScoreWidget.h"
#include "Content/GUI/PauseWidget.h"

MenuViewport::MenuViewport(int drawOrder, Screenspace screenspace) : Viewport(drawOrder, screenspace)
{
	mainMenu = new MainMenu();
	scoreWidget = new ScoreWidget();
	pauseWidget = new PauseWidget();

	elements.push_back(mainMenu);
	elements.push_back(scoreWidget);
	elements.push_back(pauseWidget);

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

void MenuViewport::ShowPauseWidget(bool value)
{
	pauseWidget->SetActive(value);
}

ScoreWidget* MenuViewport::GetScoreWidget()
{
	return scoreWidget;
}
