#include "Monster.h"

#include "../../Architecture/Components/Collider.h"
#include "../../Architecture/Components/Sprite.h"

#include "../Components/HittableComponent.h"
#include "Player.h"

Monster::Monster()
{
	hittableComp = AddComponent<HittableComponent>(4);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(20);
}

Monster::Monster(int health, float radius, int movementSpeed, int turnSpeed) : MovableObject(radius, movementSpeed, turnSpeed)
{
	hittableComp = AddComponent<HittableComponent>(health);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(radius);
}

HittableComponent* Monster::GetHittableComponent()
{
	return hittableComp;
}

void Monster::OnCollisionEnter(Object* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (player == nullptr)
	{
		return;
	}
	if (player->GetHittableComponent() == nullptr)
	{
		return;
	}
	player->GetHittableComponent()->TakeDamage(1);
}
