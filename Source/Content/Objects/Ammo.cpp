#include "Ammo.h"

#include "../../Architecture/Player.h"

Ammo::Ammo() : Pickup()
{
	textureFront = 9;
}

Ammo::Ammo(float radius, int value) : Pickup(radius, value)
{
	textureFront = 9;
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
