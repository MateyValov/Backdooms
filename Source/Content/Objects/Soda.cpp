#include "Soda.h"

#include "../../Architecture/Components/Sprite.h"

#include "../Components/HittableComponent.h"
#include "Player.h"

Soda::Soda()
{
	AddComponent<Sprite>(8);
}

Soda::Soda(int value) : Pickup(value)
{
	AddComponent<Sprite>(8);
}

void Soda::Obtain(Player* player)
{
	if (player == nullptr)
	{
		return;
	}
	if (player->GetHittableComponent() == nullptr)
	{
		return;
	}
	if(player->GetHittableComponent()->RestoreHealth(value))
	{
		Destroy();
	}
}
