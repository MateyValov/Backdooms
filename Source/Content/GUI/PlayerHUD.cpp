#include "PlayerHUD.h"

#include "../../Architecture/GUI/GUI_Image.h"

#include "../Components/HittableComponent.h"
#include "../Objects/Player.h"

PlayerHUD::PlayerHUD(Player* source)
{
	this->source = source;
	onHealthUpdated = source->GetHittableComponent()->healthUpdated.Subscribe(this, &PlayerHUD::UpdateHealth);
	onAmmoUpdated = source->ammoUpdated.Subscribe(this, &PlayerHUD::UpdateAmmo);

	healthBar = new GUI_Image(
		"GUI/Bar.png", Vector2(10.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color::Red());

	ammoBar = new GUI_Image(
		"GUI/Bar.png", Vector2(153.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color::Green());

	elements.push_back(new GUI_Image("GUI/Bar.png", Vector2(10.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color(0.3f, 0.3f, 0.3f)));
	elements.push_back(new GUI_Image("GUI/Bar.png", Vector2(153.f, 10.f), Vector2(1.f, 1.f), TOP_LEFT, Color(0.3f, 0.3f, 0.3f)));
	elements.push_back(healthBar);
	elements.push_back(ammoBar);
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(5.f, 5.f)));
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(148.f, 5.f)));
	elements.push_back(new GUI_Image("GUI/Gun.png", Vector2(-93.f, -149.f), Vector2(1.f, 1.f), BOTTON_CENTER, Color()));
	elements.push_back(new GUI_Image("GUI/Crosshair.png", Vector2(-2.f, -2.f), Vector2(1.f,1.f), MIDDLE_CENTER, Color()));
}

void PlayerHUD::UpdateHealth(float newValue, float maxValue)
{
	healthBar->scale.x = newValue / maxValue;
}

void PlayerHUD::UpdateAmmo(float newValue, float maxValue)
{
	ammoBar->scale.x = newValue / maxValue;
}


