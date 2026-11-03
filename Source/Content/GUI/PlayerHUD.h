#pragma once

#include "../../Architecture/GUI/Widget.h"
#include "../../Structs/Event.h"

class Player;
class GUI_Image;

class PlayerHUD : public Widget
{
private:
	Player* source;

	Listener onHealthUpdated;
	Listener onAmmoUpdated;

	GUI_Image* healthBar;
	GUI_Image* ammoBar;

	void UpdateHealth(float newValue, float maxValue);
	void UpdateAmmo(float newValue, float maxValue);

public:
	PlayerHUD(Player* source);
};
