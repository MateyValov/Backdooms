#pragma once

#include "Architecture/Viewport.h"

class Player;
class PlayerHUD;
class GameOverWidget;
class WaitingWidget;

class PlayerViewport : public Viewport
{
private:
	PlayerHUD* playerHUD = nullptr;
	GameOverWidget* gameOverWidget = nullptr;
	WaitingWidget* waitingWidget = nullptr;

public:
	PlayerViewport(int drawOrder, Screenspace screenspace, Player* player);

	void ShowPlayerHUD(bool value);
	void ShowGameOverWidget(bool value);
	void ShowWaitingWidget(bool value);
};
