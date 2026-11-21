#pragma once

#include "Architecture/MovableObject.h"
#include "Structs/Event.h"

class HittableComponent;
class Player;

class Monster : public MovableObject
{
private:
	HittableComponent* hittableComp;
	std::vector<Player*> players;
	Player* currentTarget = nullptr;

	Listener onHealthDepleted;

protected:
	float sightRange = 350.f;
	float loseRange = 500.f;

	virtual void OnStart() override;
	virtual void OnUpdate() override;

	void Idle();
	void Persue();

	void Die();

public:
	Monster();
	Monster(int health, float radius, int movementSpeed, int turnSpeed);

	HittableComponent* GetHittableComponent();

	void OnCollisionEnter(Object* other) override;
};
