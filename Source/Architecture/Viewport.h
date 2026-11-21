#pragma once

#include <vector>

#include "Modules/Window.h"

class GUI_Element;

class Viewport
{
protected:
	Screenspace screenspace;
	std::vector<GUI_Element*> elements;

public:
	Viewport(Screenspace screenspace);
	virtual ~Viewport() = default;

	void UpdateViewport();

	Screenspace GetScreenspace() const;
};