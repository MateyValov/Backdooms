#pragma once

#include "../../Structs/Transform.h"
#include "../../Structs/Event.h"


class PlayerController
{
private:
	unsigned char forwardKey;
	unsigned char leftKey;
	unsigned char backKey;
	unsigned char rightKey;
	unsigned char interactKey;

	Vector2 movementInput;

public:
	PlayerController(unsigned char forwardKey, unsigned char leftKey, unsigned char backKey, unsigned char rightKey, unsigned char interactKey);

	Event<> interactPressed;

	void UpdateController();

	Vector2 GetMovementInput() const;

};
