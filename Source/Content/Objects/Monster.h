#pragma once

#include "../../Architecture/MovableObject.h"
#include "Hittable.h"

class Monster : public MovableObject , public Hittable
{
public:
	Monster();
	Monster(int health, float radius, int movementSpeed, int turnSpeed);

	void OnCollisionEnter(Object* other) override;
};
