#include "Monster.h"

#include "Architecture/Components/Collider.h"
#include "Architecture/Components/Sprite.h"
#include "Architecture/Level.h"

#include "Modules/Engine.h"

#include "Content/Components/HittableComponent.h"
#include "Player.h"

Monster::Monster()
{
	hittableComp = AddComponent<HittableComponent>(4);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(20);
}

Monster::Monster(int health, float radius, int movementSpeed, int turnSpeed) : MovableObject(movementSpeed, turnSpeed)
{
	hittableComp = AddComponent<HittableComponent>(health);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(radius);
}

void Monster::OnStart()
{
	if (GetLevel() == nullptr)
	{
		return;
	}
	players = GetLevel()->GetObjectsOfClass<Player>();
}

void Monster::OnUpdate()
{
	if (currentTarget == nullptr)
	{
		Idle();
	}
	else
	{
		Persue();
	}
}

void Monster::Idle()
{
	for (Player* p : players)
	{
		if (p == nullptr)
		{
			continue;
		}
		if (Vector2::distance(p->GetTransform().position, transform.position) < sightRange)
		{
			currentTarget = p;
			return;
		}
	}
}

void Monster::Persue()
{
	if (currentTarget == nullptr)
	{
		return;
	}
	if (Vector2::distance(currentTarget->GetTransform().position, transform.position) > loseRange)
	{
		currentTarget = nullptr;
	}
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
