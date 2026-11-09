#include "Pickup.h"

#include "../../Architecture/Components/Collider.h"

Pickup::Pickup(float radius)
{
	AddComponent<Collider>(radius);
}

Pickup::Pickup(float radius, int value)
{
	AddComponent<Collider>(radius);
	this->value = value;
}

void Pickup::SetValue(int value)
{
	this->value = value;
}

int Pickup::GetValue() const
{
	return value;
}
