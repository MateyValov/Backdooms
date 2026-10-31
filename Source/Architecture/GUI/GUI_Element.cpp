#include "GUI_Element.h"

GUI_Element::GUI_Element()
{
	this->position = Vector2();
	this->alignment = TOP_LEFT;
	this->color = Color();
}

GUI_Element::GUI_Element(Vector2 position)
{
	this->position = position;
	this->alignment = TOP_LEFT;
	this->color = Color();
}

GUI_Element::GUI_Element(Vector2 position, Vector2 alignment, Color color)
{
	this->position = position;
	this->alignment = alignment;
	this->color = color;
}

Vector2 GUI_Element::GetTruePosition(Vector2 TopLeft, Vector2 BottomRight)
{
	int screenW = BottomRight.x - TopLeft.x;
	int screenH = BottomRight.y - TopLeft.y;

	return Vector2(position.x + (alignment.x + 1) * (screenW / 2) + TopLeft.x,
		position.x + (alignment.x + 1) * (screenH / 2) + TopLeft.y);
}
