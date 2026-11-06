#include "Pickup.h"

Pickup::Pickup(int value)
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
