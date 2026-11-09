#pragma once

#include "MovableObject.h"

class Controller;
class Viewport;

class PlayerBase : public MovableObject
{
protected:
	Controller* controller = nullptr;
	Viewport* viewport = nullptr;

protected:
	virtual void OnUpdate() override;

public:
	PlayerBase();
	PlayerBase(float radius, int movementSpeed, int turnSpeed);

	void AssignController(Controller* controller);
	void AssignViewport(Viewport* viewport);

};
