#include "Soda.h"

#include "../../Architecture/Player.h"

Soda::Soda() : Pickup()
{
	textureFront = 8;
}

Soda::Soda(float radius, int value) : Pickup(radius, value)
{
	textureFront = 8;
}

void Soda::Obtain(Player* player)
{
	if (player == nullptr)
	{
		return;
	}
	if(player->AddHealth(value))
	{
		Destroy();
	}
}
