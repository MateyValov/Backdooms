#pragma once

#include "Object.h"

#include "../Structs/Transform.h"

class MovableObject : public Object
{
private:
	int movementSpeed;
	int turnSpeed;

public:
	MovableObject();
	MovableObject(float radius, int movementSpeed, int turnSpeed);

	void Turn(bool isRight);
	void Move(Vector2 direction);
};