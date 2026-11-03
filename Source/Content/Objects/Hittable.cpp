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
	healthUpdated.Invoke(health, maxHealth);

	if (health == 0)
	{
		isKilled = true;
		onKilled.Invoke();
	}
}

void Hittable::SetMaxHealth(int value, bool heal)
{
	maxHealth = value;
	if (heal)
	{
		health = maxHealth;
	}
	healthUpdated.Invoke(health, maxHealth);
}

bool Hittable::RestoreHealth(unsigned int amount)
{
	int before = health;
	SetHealth(health + amount);
	return before != health;
}

void Hittable::TakeDamage(unsigned int amount)
{
	SetHealth(health - amount);
}

bool Hittable::IsKilled() const
{
	return isKilled;
}
