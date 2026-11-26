#pragma once

#include "Architecture/MovableObject.h"

#include "Structs/Event.h"

class HittableComponent;
class PlayerController;

class Player : public MovableObject
{
private:
	int ammo;
	int maxAmmo;

	HittableComponent* hittableComp;

	void InitRenderer();

public:
	Player();
	Player(int health, int ammo);
	Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed);

	Event<int, int> ammoUpdated;
	Event<> exitTouched;

	void OnCollisionEnter(Object* other) override;
	void OnWallEnter(int tileX, int tileY, int wallValue) override;

	HittableComponent* GetHittableComponent();

	int GetAmmo() const;
	int GetMaxAmmo() const;

	void SetAmmo(int value);
	void SetMaxAmmo(int value, bool restore = true);

	bool AddAmmo(unsigned int amount);

	void InteractPressed();
};