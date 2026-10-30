#include "GUI_Element.h"

GUI_Element::GUI_Element(Vector2 position)
{
	this->position = position;
}

GUI_Element::GUI_Element(Vector2 position, Vector2 alignment, Color color)
{
	this->position = position;
	this->alignment = alignment;
	this->color = color;
}

Vector2 GUI_Element::GetTruePosition(int screenspaceX, int screenspaceY)
{
	return Vector2(position.x + (alignment.x + 1) * (screenspaceX / 2), 
		position.x + (alignment.x + 1) * (screenspaceY / 2));
}
