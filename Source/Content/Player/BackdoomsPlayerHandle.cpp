#include "BackdoomsPlayerHandle.h"

#include "Modules/Input.h"
#include "Modules/Engine.h"

#include "Architecture/Level.h"
#include "Architecture/Camera.h"

#include "Content/Objects/Player.h"
#include "Content/GUI/PlayerViewport.h"
#include "Content/Components/HittableComponent.h"


BackdoomsPlayerHandle::BackdoomsPlayerHandle(Screenspace screenspace, ControlScheme controlScheme) : PlayerHandle(screenspace)
{
	this->forwardKey  = controlScheme.forwardKey;
	this->leftKey     = controlScheme.leftKey;
	this->backKey     = controlScheme.backKey;
	this->rightKey    = controlScheme.rightKey;
	this->interactKey = controlScheme.interactKey;
}

void BackdoomsPlayerHandle::OnPlayerDied()
{
	PlayerViewport* pv = dynamic_cast<PlayerViewport*>(viewport);
	if (IsValid(pv) == false)
	{
		return;
	}

	pv->ShowPlayerHUD(false);
	pv->ShowGameOverWidget(true);
	pv->ShowWaitingWidget(false);

	playerObject->Destroy();
}

void BackdoomsPlayerHandle::OnExitReached()
{
	PlayerViewport* pv = dynamic_cast<PlayerViewport*>(viewport);
	if (IsValid(pv) == false)
	{
		return;
	}

	pv->ShowPlayerHUD(false);
	pv->ShowGameOverWidget(false);
	pv->ShowWaitingWidget(true);
}

void BackdoomsPlayerHandle::OnRespawned()
{
	PlayerViewport* pv = dynamic_cast<PlayerViewport*>(viewport);
	if (IsValid(pv) == false)
	{
		return;
	}

	pv->ShowPlayerHUD(true);
	pv->ShowGameOverWidget(false);
	pv->ShowWaitingWidget(false);
}

void BackdoomsPlayerHandle::OnStart()
{
	Player* player = GetLevel()->Spawn<Player>(Vector2(), 90.f);
	playerObject = player;
	playerRef = player;

	camera = GetLevel()->Spawn<Camera>(Vector2(), 90.f, screenspace, 60, 20, 2, 1.f);
	camera->SetParent(player);

	viewport = GetLevel()->Spawn<PlayerViewport>(Vector2(), 90.f, 0, screenspace, player);

	onPlayerDied += player->GetHittableComponent()->onKilled.Subscribe(this, &BackdoomsPlayerHandle::OnPlayerDied);
	onExitReached += player->exitTouched.Subscribe(this, &BackdoomsPlayerHandle::OnExitReached);
}

void BackdoomsPlayerHandle::OnUpdate()
{
	PlayerHandle::OnUpdate();

	if (playerRef->IsActive() == false)
	{
		return;
	}

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

	if (Input::GetKeyState(interactKey) == pressed && Engine::GetTimeScale() != 0.f)
	{
		playerRef->InteractPressed();
	}
}




