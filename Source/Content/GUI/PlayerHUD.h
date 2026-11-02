#pragma once

#include "../../Architecture/GUI/Widget.h"

class Player;
class GUI_Image;

class PlayerHUD : public Widget
{
private:
	Player* source;

	GUI_Image* healthBar;
	GUI_Image* ammoBar;

	void OnHealthUpdated(int newValue);
	void OnAmmoUpdated(int newValue);

public:
	PlayerHUD(Player* source);
};
