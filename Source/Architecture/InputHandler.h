#pragma once

#include <vector>

#include "Structs/Event.h"

struct InputAction
{
	const unsigned char key;
	//Event<> onKeyPressed;
	//Event<> onKeyHeld;
	//Event<> onKeyReleased;

	bool disabled = false;

	InputAction(unsigned char key) : key(key) {}
};

class InputHandler
{
private:
	std::vector<InputAction> inputActions;

public:
	void UpdateActions();
	void RegisterAction(InputAction action);
};
