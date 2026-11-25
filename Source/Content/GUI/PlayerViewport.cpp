#include "PlayerViewport.h"

#include "Content/GUI/PlayerHUD.h"
#include "Content/GUI/GameOverWidget.h"
#include "Content/GUI/WaitingWidget.h"

PlayerViewport::PlayerViewport(int drawOrder, Screenspace screenspace, Player* player) : Viewport(drawOrder, screenspace)
{
	playerHUD = new PlayerHUD(player);
	gameOverWidget = new GameOverWidget();
	waitingWidget = new WaitingWidget();

	elements.push_back(playerHUD);
	elements.push_back(gameOverWidget);
	elements.push_back(waitingWidget);

	ShowPlayerHUD(true);
}

void PlayerViewport::ShowPlayerHUD(bool value)
{
	playerHUD->SetActive(value);
}

void PlayerViewport::ShowGameOverWidget(bool value)
{
	gameOverWidget->SetActive(value);
}

void PlayerViewport::ShowWaitingWidget(bool value)
{
	waitingWidget->SetActive(value);
}
