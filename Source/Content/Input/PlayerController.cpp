#include "PlayerController.h"

#include "../../Modules/Input.h"

PlayerController::PlayerController(ControlScheme controlScheme)
{
	this->forwardKey = controlScheme.forwardKey;
	this->leftKey = controlScheme.leftKey;
	this->backKey = controlScheme.backKey;
	this->rightKey = controlScheme.rightKey;
	this->interactKey = controlScheme.interactKey;
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
