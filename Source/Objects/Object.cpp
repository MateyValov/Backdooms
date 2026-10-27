#include "Object.h"

#include "../Structs/MathFuncs.h"
#include "../Engine/Engine.h"
#include "Map.h";

Object::Object()
{
	radius = 20.f;
	hasCollision = false;
}

Object::Object(float radius)
{
	this->radius = radius;
}

Transform Object::GetTransform()
{
	return transform;
}

float Object::GetRadius() const
{
	return radius;
}

bool Object::HasCollision() const
{
	return hasCollision;
}

void Object::SetTransform(Transform newTrasform)
{
	SetPosition(newTrasform.position);
	SetAngle(newTrasform.angle);
}

void Object::SetPosition(Vector2 newPosition)
{
	transform.position = newPosition;
	Engine::GetMap()->AcknowledgePosition(this);
}

void Object::SetAngle(float newAngle)
{
	transform.angle = clampAngle(newAngle);
}