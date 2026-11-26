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
	AddComponent<Sprite>(20, 21, 22, 23);
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
	EvaluateTarget();
	if (IsValid(currentTarget) == false)
	{
		IdleState();
		return;
	}
	if (IsValid(currentTarget) && Vector2::distance(transform.position, currentTarget->GetTransform().position) <= attackRange)
	{
		AttackState();
		return;
	}
	if (IsValid(currentTarget) && Vector2::distance(transform.position, currentTarget->GetTransform().position) <= loseRange)
	{
		ChaseState();
		return;
	}
}

void Monster::EvaluateTarget()
{
	if (IsValid(currentTarget) == false)
	{
		return;
	}
	if (Vector2::distance(currentTarget->GetTransform().position, transform.position) > loseRange)
	{
		currentTarget = nullptr;
	}
}

void Monster::IdleState()
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

void Monster::ChaseState()
{
	float angleToTarget = Transform::angleToSigned(transform, currentTarget->GetTransform().position);
	Turn(angleToTarget > 0.f);
	
	if (fabsf(angleToTarget) > targetAngle)
	{
		return;
	}

	Move(transform.GetForward());
}

void Monster::AttackState()
{
	if (IsValid(currentTarget) == false || currentTarget->GetHittableComponent() == nullptr)
	{
		return;
	}

	attackCooldown -= Engine::GetDeltaTime();
	if (attackCooldown > 0.f)
	{
		return;
	}

	attackCooldown = attackInterval;
	currentTarget->GetHittableComponent()->TakeDamage(1);
	Audio::PlaySoundEffect("monsterAttack.wav");
}

void Monster::Die()
{
	Destroy();
}

HittableComponent* Monster::GetHittableComponent()
{
	return hittableComp;
}
