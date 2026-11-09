#pragma once

#include "../../Structs/Transform.h"
#include "../../Structs/Event.h"


struct ControlScheme
{
	unsigned char forwardKey;
	unsigned char leftKey;
	unsigned char backKey;
	unsigned char rightKey;
	unsigned char interactKey;

	ControlScheme(unsigned char forwardKey, unsigned char leftKey, unsigned char backKey, unsigned char rightKey, unsigned char interactKey)
	{
		this->forwardKey = forwardKey;
		this->leftKey = leftKey;
		this->backKey = backKey;
		this->rightKey = rightKey;
		this->interactKey = interactKey;
	}
	
	static ControlScheme PlayerOne()
	{
		return ControlScheme('W', 'A', 'S', 'D', 'Q');
	}

	static ControlScheme PlayerTwo()
	{
		return ControlScheme(0x26, 0x25, 0x28, 0x27, 0x20);
	}

	static ControlScheme PlayerThree()
	{
		return ControlScheme('8', '4', '2', '6', '7');
	}
};

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
	PlayerController(ControlScheme controlScheme);

	Event<> interactPressed;

	void UpdateController();

	Vector2 GetMovementInput() const;

};
