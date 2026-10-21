#pragma once

#include "MovableObject.h"
#include "../Utility/UtilityStructs.h"

#define PLAYER_MAX_HEALTH 8
#define PLAYER_MAX_AMMO 8

class Player : public MovableObject
{
private:
	int health;
	int ammo;

public:
	Player();
	Player(float radius, int movementSpeed, int turnSpeed);

	virtual void Start() override;
	virtual void Update() override;

	int GetHealth();
	int GetAmmo();
};