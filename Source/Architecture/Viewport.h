#pragma once

#include "Object.h"

#include <vector>

#include "Modules/Window.h"

class GUI_Element;

class Viewport : public Object
{
private:
	void UpdateViewport();

	friend class Window;

protected:
	Screenspace screenspace;
	std::vector<GUI_Element*> elements;

public:
	Viewport(Screenspace screenspace);
	virtual ~Viewport() = default;

	Screenspace GetScreenspace() const;
};