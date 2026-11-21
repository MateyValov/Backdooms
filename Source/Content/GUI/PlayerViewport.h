#pragma once

#include "Architecture/Viewport.h"

class Player;
class PlayerHUD;
class GameOverWidget;

class PlayerViewport : public Viewport
{
private:
	PlayerHUD* playerHUD = nullptr;
	GameOverWidget* gameOverWidget = nullptr;

public:
	PlayerViewport(Player* player, Screenspace screenspace);

	void ShowPlayerHUD(bool value);
	void ShowGameOverWidget(bool value);
};
