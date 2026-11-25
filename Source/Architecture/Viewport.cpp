#include "Viewport.h"

#include "Architecture/GUI/GUI_Element.h"

Viewport::Viewport(int drawOrder, Screenspace screenspace)
{
	this->drawOrder = drawOrder;
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

int Viewport::GetDrawOrder() const
{
	return drawOrder;
}

void Viewport::SetDrawOrder(int value)
{
	drawOrder = value;
}


