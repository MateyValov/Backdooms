#include "Object.h"

#include "../Structs/MathFuncs.h"
#include "../Engine/Engine.h"
#include "Map.h";

Object::Object()
{
	radius = 20.f;
	hasCollision = true;
}

Object::Object(float radius)
{
	this->radius = radius;
	hasCollision = true;
}

Transform Object::GetTransform() const
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
	transform.position = hasCollision ? Engine::GetMap()->EvaluatePosition(newPosition, radius) : newPosition;
}

void Object::SetAngle(float newAngle)
{
	transform.angle = clampAngle(newAngle);
}