#include "Collider.h"


Collider::Collider(float radius)
{
	this->radius = radius;
}

bool Collider::IsActive()
{
	return isActive;
}

void Collider::SetActive(bool value)
{
	isActive = value;
}

float Collider::GetRadius() const
{
	return radius;
}