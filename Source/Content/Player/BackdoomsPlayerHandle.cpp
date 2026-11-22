#include "BackdoomsPlayerHandle.h"

#include "Modules/Input.h"

#include "Architecture/Level.h"
#include "Architecture/Camera.h"

#include "Content/Objects/Player.h"
#include "Content/GUI/PlayerViewport.h"

BackdoomsPlayerHandle::BackdoomsPlayerHandle(Screenspace screenspace, ControlScheme controlScheme) : PlayerHandle(screenspace)
{
	this->forwardKey  = controlScheme.forwardKey;
	this->leftKey     = controlScheme.leftKey;
	this->backKey     = controlScheme.backKey;
	this->rightKey    = controlScheme.rightKey;
	this->interactKey = controlScheme.interactKey;
}

void BackdoomsPlayerHandle::OnStart()
{
	Player* player = GetLevel()->Spawn<Player>(Vector2(), 90.f);
	playerObject = player;
	playerRef = player;

	viewport = new PlayerViewport(player, screenspace);

	camera = GetLevel()->Spawn<Camera>(Vector2(), 90.f, screenspace, 60, 20, 2, 1.f);
	camera->SetParent(player);
}

void BackdoomsPlayerHandle::OnUpdate()
{
	PlayerHandle::OnUpdate();

	if (playerRef == nullptr)
	{
		return;
	}

	int forward = Input::GetKeyState(forwardKey) == held;
	int back = Input::GetKeyState(backKey) == held;
	int left = Input::GetKeyState(leftKey) == held;
	int right = Input::GetKeyState(rightKey) == held;

	movementInput.x = left - right;
	movementInput.y = forward - back;

	if (movementInput.x != 0.f)
	{
		playerRef->Turn(movementInput.x > 0);
	}

	playerRef->Move(playerRef->GetTransform().GetForward() * movementInput.y);

	if (Input::GetKeyState(interactKey) == pressed)
	{
		playerRef->InteractPressed();
	}
}




