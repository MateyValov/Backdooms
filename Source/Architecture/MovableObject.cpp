#include "MovableObject.h"

#include "../Modules/Engine.h"

MovableObject::MovableObject() : Object()
{
	movementSpeed = 200;
	turnSpeed = 150;
}

MovableObject::MovableObject(float radius, int movementSpeed, int turnSpeed) : Object(radius)
{
	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;
}

void MovableObject::Turn(bool isRight) 
{
	SetAngle(transform.angle + (isRight ? 1.f : -1.f) * turnSpeed * Engine::GetDeltaTime());
}

void MovableObject::Move(Vector2 direction)
{
	SetPosition(transform.position + direction * movementSpeed * Engine::GetDeltaTime());
}
