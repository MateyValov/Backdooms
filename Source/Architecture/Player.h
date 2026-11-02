#pragma once

#include "MovableObject.h"

class Player : public MovableObject
{
private:
	int health;
	int maxHealth;

	int ammo;
	int maxAmmo;

public:
	Player();
	Player(int health, int ammo);
	Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed);

	void OnCollisionEnter(Object* other) override;
	void OnWallEnter(int tileX, int tileY, int wallValue) override;

	int GetHealth() const;
	int GetMaxHealth() const;
	int GetAmmo() const;
	int GetMaxAmmo() const;

	void AddHealth(int Amount);
	void AddAmmo(int Amount);

	void InteractPressed();
};