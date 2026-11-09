#include "Viewport.h"


Viewport::Viewport(Screenspace screenspace)
{
	this->screenspace = screenspace;
}

Screenspace Viewport::GetScreenspace() const
{
	return screenspace;
}

void Viewport::UpdatViewport()
{
}


