#include "Ammo.h"

#include "../../Architecture/Components/Sprite.h"

#include "Player.h"

Ammo::Ammo() : Pickup()
{
	AddComponent<Sprite>(9);
}

Ammo::Ammo(int value) : Pickup(value)
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
		Destroy();
	}
}
