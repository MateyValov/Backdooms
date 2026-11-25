#include "Ammo.h"

#include "Architecture/Components/Sprite.h"
#include "Modules/Audio.h"

#include "Player.h"

Ammo::Ammo(float radius) : Pickup(radius)
{
	AddComponent<Sprite>(9);
}

Ammo::Ammo(float radius, int value) : Pickup(radius, value)
{
	AddComponent<Sprite>(9);
}

void Ammo::Obtain(Player* player)
{
	if (player == nullptr)
	{
		return;
	}
	if (player->AddAmmo(value))
	{
		Audio::PlaySoundEffect("pickup.wav");
		Destroy();
	}
}
