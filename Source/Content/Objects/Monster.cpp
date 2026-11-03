#include "Monster.h"

#include "Player.h"

Monster::Monster() : Hittable(4)
{
	SetTextureId(20);
	radius = 30;
}

Monster::Monster(int health, float radius, int movementSpeed, int turnSpeed) : Hittable(health), MovableObject(radius, movementSpeed, turnSpeed)
{
	SetTextureId(20);
}

void Monster::OnCollisionEnter(Object* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player != nullptr)
	{
		player->TakeDamage(1);
	}
}
