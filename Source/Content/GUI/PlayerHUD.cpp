#include "PlayerHUD.h"

#include "../../Architecture/GUI/GUI_Image.h"
#include "../../Architecture/Player.h"

PlayerHUD::PlayerHUD(Player* source)
{
	this->source = source;

	healthBar = new GUI_Image(
		"GUI/Bar.png", Vector2(10.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color::Red());

	ammoBar = new GUI_Image(
		"GUI/Bar.png", Vector2(153.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color::Green());

	elements.push_back(healthBar);
	elements.push_back(ammoBar);
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(5.f, 5.f)));
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(148.f, 5.f)));
	elements.push_back(new GUI_Image("GUI/Crosshair.png", Vector2(-2.f, -2.f), Vector2(1.f,1.f), MIDDLE_CENTER, Color()));
}

void PlayerHUD::OnHealthUpdated(int newValue)
{
	healthBar->scale.x = float(newValue / source->GetMaxHealth());
}

void PlayerHUD::OnAmmoUpdated(int newValue)
{
	ammoBar->scale.x = float(newValue / source->GetMaxAmmo());
}


