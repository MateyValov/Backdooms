#include "Pickup.h"

Pickup::Pickup(float radius, int value) : Object(radius)
{
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
