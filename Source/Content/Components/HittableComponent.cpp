#include "HittableComponent.h"

#include "../../Structs/MathFuncs.h"

HittableComponent::HittableComponent(int health)
{
	this->maxHealth = health;
	this->health = health;
}

int HittableComponent::GetHealth() const
{
	return health;
}

int HittableComponent::GetMaxHealth() const
{
	return maxHealth;
}

void HittableComponent::SetHealth(int value)
{
	health = clamp(value, 0, maxHealth);
	healthUpdated.Invoke(health, maxHealth);

	if (health == 0)
	{
		isKilled = true;
		onKilled.Invoke();
	}
}

void HittableComponent::SetMaxHealth(int value, bool heal)
{
	maxHealth = value;
	if (heal)
	{
		health = maxHealth;
	}
	healthUpdated.Invoke(health, maxHealth);
}

bool HittableComponent::RestoreHealth(unsigned int amount)
{
	int before = health;
	SetHealth(health + amount);
	return before != health;
}

void HittableComponent::TakeDamage(unsigned int amount)
{
	SetHealth(health - amount);
}

bool HittableComponent::IsKilled() const
{
	return isKilled;
}
