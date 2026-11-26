#pragma once

#include "Architecture/MovableObject.h"
#include "Structs/Event.h"

class HittableComponent;
class Player;

class Monster : public MovableObject
{
private:
	float attackCooldown = 0.f;
	float attackInterval;

	HittableComponent* hittableComp;
	std::vector<Player*> players;
	Player* currentTarget = nullptr;

	Listener onHealthDepleted;

	void InitRenderer();

protected:
	float sightRange = 350.f;
	float attackRange = 64.f;
	float loseRange = 500.f;
	float targetAngle = 30.f;

	virtual void OnStart() override;
	virtual void OnUpdate() override;

	void EvaluateTarget();
	void IdleState();
	void ChaseState();
	void AttackState();

	void Die();

public:
	Monster();
	Monster(int health, float attackInterval, float radius, int movementSpeed, int turnSpeed);

	HittableComponent* GetHittableComponent();
};
