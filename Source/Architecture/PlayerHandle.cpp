#include "PlayerHandle.h"

#include "Viewport.h"


void PlayerHandle::OnUpdate()
{
	if (viewport != nullptr)
	{
		viewport->UpdateViewport();
	}
}

void PlayerHandle::AssignObject(Object* playerObject)
{
	this->playerObject = playerObject;
}

void PlayerHandle::AssignViewport(Viewport* viewport)
{
	this->viewport = viewport;
}

Object* PlayerHandle::GetPlayerObject()
{
	return playerObject;
}

Viewport* PlayerHandle::GetPlayerViewport()
{
	return viewport;
}
