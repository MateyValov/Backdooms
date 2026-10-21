#include "Object.h"

#include <math.h>


Object::Object()
{
	radius = 20.f;
}

Object::Object(float radius)
{
	this->radius = radius;
}

Transform Object::GetTransform() const
{
	return transform;
}

float Object::GetRadius() const
{
	return radius;
}

void Object::SetTransform(Transform newTrasform)
{
	SetPosition(newTrasform.position);
	SetAngle(newTrasform.angle);
}

void Object::SetPosition(Vector2 newPosition)
{
	transform.position = newPosition;
}

void Object::SetAngle(float newAngle)
{
	transform.angle = FixAng(newAngle);
}