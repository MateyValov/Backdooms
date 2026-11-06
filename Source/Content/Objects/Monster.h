#pragma once

#include "../../Architecture/MovableObject.h"

class HittableComponent;

class Monster : public MovableObject
{
private:
	HittableComponent* hittableComp;

public:
	Monster();
	Monster(int health, float radius, int movementSpeed, int turnSpeed);

	HittableComponent* GetHittableComponent();

	void OnCollisionEnter(Object* other) override;
};
