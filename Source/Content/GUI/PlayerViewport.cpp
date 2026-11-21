#include "PlayerViewport.h"

#include "Content/GUI/PlayerHUD.h"
#include "Content/GUI/GameOverWidget.h"

PlayerViewport::PlayerViewport(Player* player, Screenspace screenspace) : Viewport(screenspace)
{
	playerHUD = new PlayerHUD(player);
	gameOverWidget = new GameOverWidget();

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
