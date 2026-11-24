#include "PlayerHandle.h"

#include "Viewport.h"
#include "Camera.h"


PlayerHandle::PlayerHandle(Screenspace screenspace)
{
	this->screenspace = screenspace;
}

void PlayerHandle::AssignObject(Object* playerObject)
{
	this->playerObject = playerObject;
}

void PlayerHandle::AssignCamera(Camera* camera)
{
	this->camera = camera;
}

void PlayerHandle::AssignViewport(Viewport* viewport)
{
	this->viewport = viewport;
}

Object* PlayerHandle::GetPlayerObject()
{
	return playerObject;
}

Camera* PlayerHandle::GetCamera()
{
	return camera;
}

Viewport* PlayerHandle::GetPlayerViewport()
{
	return viewport;
}
