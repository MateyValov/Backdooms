#include "Monster.h"

#include "Architecture/Components/Collider.h"
#include "Architecture/Components/Sprite.h"
#include "Architecture/Level.h"

#include "Modules/Audio.h"
#include "Modules/Engine.h"

#include "Content/Components/HittableComponent.h"
#include "Player.h"

Monster::Monster()
{
	attackInterval = 1.f;

	hittableComp = AddComponent<HittableComponent>(3);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(20);

	onHealthDepleted = hittableComp->onKilled.Subscribe(this, &Monster::Die);
}

Monster::Monster(int health, float attackInterval, float radius, int movementSpeed, int turnSpeed) : MovableObject(movementSpeed, turnSpeed)
{
	this->attackInterval = attackInterval;

	hittableComp = AddComponent<HittableComponent>(health);
	AddComponent<Sprite>(20);
	AddComponent<Collider>(radius);

	onHealthDepleted = hittableComp->onKilled.Subscribe(this, &Monster::Die);
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
	if (IsValid(currentTarget))
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
	size_t n = players.size();
	for (size_t i = 0; i < n; i++)
	{
		if (IsValid(players[i]) == false)
		{
			continue;
		}
		if (Vector2::distance(players[i]->GetTransform().position, transform.position) < sightRange)
		{
			Audio::PlaySoundEffect("monster.wav");
			currentTarget = players[i];
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

void Monster::Die()
{
	Destroy();
}

HittableComponent* Monster::GetHittableComponent()
{
	return hittableComp;
}

void Monster::OnCollisionEnter(Object* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (IsValid(player) == false)
	{
		return;
	}
	isAttacking = true;
	timeElapsed = attackInterval;
}

void Monster::OnCollisionStay(Object* other)
{
	if (isAttacking == false)
	{
		return;
	}
	timeElapsed += Engine::GetDeltaTime();

	if (timeElapsed < attackInterval)
	{
		return;
	}
	timeElapsed = 0.f;

	Player* player = dynamic_cast<Player*>(other);
	if (IsValid(player) == false)
	{
		return;
	}
	if (player->GetHittableComponent() == nullptr)
	{
		return;
	}

	Audio::PlaySoundEffect("monsterAttack.wav");
	player->GetHittableComponent()->TakeDamage(1);
}

void Monster::OnCollisionExit(Object* other)
{
	Player* player = dynamic_cast<Player*>(other);
	if (IsValid(player) == false)
	{
		return;
	}
	isAttacking = false;
}
