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
		"GUI/Bar.png", GUITransform(Vector2(10.f, 10.f), 128.f, 30.f), Color::Red());

	ammoBar = new GUI_Image(
		"GUI/Bar.png", GUITransform(Vector2(153.f, 10.f), 128.f, 30.f), Color::Green());


	elements.push_back(new GUI_Image("GUI/Bar.png", GUITransform(Vector2(10.f, 10.f), 128.f, 30.f), Color(0.3f, 0.3f, 0.3f)));
	elements.push_back(new GUI_Image("GUI/Bar.png", GUITransform(Vector2(153.f, 10.f), 128.f, 30.f), Color(0.3f, 0.3f, 0.3f)));
	elements.push_back(healthBar);
	elements.push_back(ammoBar);
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(5.f, 5.f), 138.f, 40.f));
	elements.push_back(new GUI_Image("GUI/BarOverlay.png", Vector2(148.f, 5.f), 138.f, 40.f));
	elements.push_back(new GUI_Image("GUI/Gun.png", GUITransform(Vector2(), 186.f, 149.f, Vector2(1.f), BOTTOM_CENTER, BOTTOM_CENTER), Color()));
	elements.push_back(new GUI_Image("GUI/Crosshair.png", GUITransform(Vector2(), 8.f, 8.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color()));
}

void PlayerHUD::UpdateHealth(float newValue, float maxValue)
{
	healthBar->transform.scale.x = newValue / maxValue;
}

void PlayerHUD::UpdateAmmo(float newValue, float maxValue)
{
	ammoBar->transform.scale.x = newValue / maxValue;
}


