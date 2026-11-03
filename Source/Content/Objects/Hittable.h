#pragma once

class Hittable
{
protected:
	int health;
	int maxHealth;

public:
	Hittable(int health);

	int GetHealth() const;
	int GetMaxHealth() const;

	void SetHealth(int value);
	void SetMaxHealth(int value, bool heal);
	void AddHealth(int Amount);
};
