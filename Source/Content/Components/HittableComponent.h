#pragma once

#include "../../Architecture/Components/Component.h"

#include "../../Structs/Event.h"

class HittableComponent : public Component
{
protected:
	int health;
	int maxHealth;

	bool isKilled = false;

public:
	HittableComponent(int health);

	Event<int, int> healthUpdated;
	Event<> onKilled;

	int GetHealth() const;
	int GetMaxHealth() const;

	void SetHealth(int value);
	void SetMaxHealth(int value, bool heal);

	bool RestoreHealth(unsigned int amount);
	void TakeDamage(unsigned int amount);

	bool IsKilled() const;
};
