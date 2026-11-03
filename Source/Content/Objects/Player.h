#pragma once

#include "../../Architecture/MovableObject.h"
#include "Hittable.h"

#include "../../Structs/Event.h"

class PlayerController;

class Player : public MovableObject, public Hittable
{
private:
	int ammo;
	int maxAmmo;

	PlayerController* controller = nullptr;

	Listener onInteractPressed;

public:
	Player();
	Player(int health, int ammo);
	Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed);

	Event<int, int> ammoUpdated;

	void Update() override;

	void OnCollisionEnter(Object* other) override;
	void OnWallEnter(int tileX, int tileY, int wallValue) override;

	void AssignController(PlayerController* controller);

	int GetAmmo() const;
	int GetMaxAmmo() const;

	void SetAmmo(int value);
	void SetMaxAmmo(int value, bool restore = true);

	bool AddAmmo(unsigned int amount);

	void InteractPressed();
};