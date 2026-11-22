#include "Viewport.h"

#include "Architecture/GUI/GUI_Element.h"

Viewport::Viewport(Screenspace screenspace)
{
	this->screenspace = screenspace;
}

void Viewport::UpdateViewport()
{
	for (GUI_Element* e : elements)
	{
		if (e->IsActive() == false)
		{
			continue;
		}
		e->Draw(screenspace);
	}
}

Screenspace Viewport::GetScreenspace() const
{
	return screenspace;
}


