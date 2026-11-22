#include "Collider.h"


Collider::Collider(float radius)
{
	this->radius = radius;
}

float Collider::GetRadius() const
{
	return radius;
}