#include "PlayerController.h"

#include "../../Modules/Input.h"

PlayerController::PlayerController(unsigned char forwardKey, unsigned char leftKey, unsigned char backKey, unsigned char rightKey, unsigned char interactKey)
{
	this->forwardKey = forwardKey;
	this->leftKey = leftKey;
	this->backKey = backKey;
	this->rightKey = rightKey;
	this->interactKey = interactKey;
}

void PlayerController::UpdateController()
{
	int forward = Input::GetKeyState(forwardKey) == held;
	int back = Input::GetKeyState(backKey) == held;
	int left = Input::GetKeyState(leftKey) == held;
	int right = Input::GetKeyState(rightKey) == held;

	movementInput.x = left - right;
	movementInput.y = forward - back;

	if (Input::GetKeyState(interactKey) == pressed)
	{
		interactPressed.Invoke();
	}
}

Vector2 PlayerController::GetMovementInput() const
{
	return movementInput;
}
