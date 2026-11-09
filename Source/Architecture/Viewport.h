#pragma once

#include "../Modules/Window.h"

class Viewport
{
private:
	Screenspace screenspace;

	void UpdatViewport();

public:
	Viewport(Screenspace screenspace);
	Screenspace GetScreenspace() const;
};