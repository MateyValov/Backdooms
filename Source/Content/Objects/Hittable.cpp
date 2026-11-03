#include "Hittable.h"

#include "../../Structs/MathFuncs.h"

Hittable::Hittable(int health)
{
	this->maxHealth = health;
	this->health = health;
}

int Hittable::GetHealth() const
{
	return health;
}

int Hittable::GetMaxHealth() const
{
	return maxHealth;
}

void Hittable::SetHealth(int value)
{
	health = clamp(value, 0, maxHealth);
}

void Hittable::SetMaxHealth(int value, bool heal)
{
	maxHealth = value;
	if (heal)
	{
		health = maxHealth;
	}
}

void Hittable::AddHealth(int Amount)
{
	health = clamp(health + Amount, 0, maxHealth);
}