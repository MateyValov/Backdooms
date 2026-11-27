#include "Monster.h"

#include "Architecture/Components/AnimatedSprite.h"
#include "Architecture/Components/Collider.h"
#include "Architecture/Level.h"

#include "Modules/Audio.h"
#include "Modules/Engine.h"

#include "Content/Components/AnimationHandlerComponent.h"
#include "Content/Components/HittableComponent.h"
#include "Player.h"


void Monster::InitRenderer()
{
	AnimatedSprite* renderer = AddComponent<AnimatedSprite>(Animation(std::vector<Frame>
	{
		Frame(20, 21, 22, 23)
	}));
	renderer->AddAnimation(Animation(std::vector<Frame>
	{
		Frame(28, 29, 30, 31),
		Frame(36, 37, 38, 39),
		Frame(44, 45, 46, 47),
		Frame(52, 53, 54, 55),
	}), "walk");
}

Monster::Monster()
{
	attackInterval = 1.f;
	movementSpeed = 180;

	hittableComp = AddComponent<HittableComponent>(3);
	InitRenderer();
	AddComponent<Collider>(20);
	AddComponent<AnimationHandlerComponent>();

	onHealthDepleted = hittableComp->onKilled.Subscribe(this, &Monster::Die);
}

Monster::Monster(int health, float attackInterval, float radius, int movementSpeed, int turnSpeed) : MovableObject(movementSpeed, turnSpeed)
{
	this->attackInterval = attackInterval;

	hittableComp = AddComponent<HittableComponent>(health);
	InitRenderer();
	AddComponent<Collider>(radius);
	AddComponent<AnimationHandlerComponent>();

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
