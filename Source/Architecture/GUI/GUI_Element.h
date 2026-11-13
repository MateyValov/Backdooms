#pragma once

#include "Modules/Window.h"

#include "Structs/GUITransform.h"
#include "Structs/Image.h"



class GUI_Element
{
public:
	GUITransform transform;
	Color color;

	GUI_Element();
	GUI_Element(Vector2 position);
	GUI_Element(Vector2 position, float width, float height);
	GUI_Element(GUITransform transform, Color color);

	virtual void Draw(Screenspace screenspace) = 0;
};