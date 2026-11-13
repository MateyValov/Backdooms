#include "GUI_Element.h"

GUI_Element::GUI_Element()
{
	this->transform = GUITransform();
	this->color = Color();
}

GUI_Element::GUI_Element(Vector2 position)
{
	this->transform = GUITransform(position);
	this->color = Color();
}

GUI_Element::GUI_Element(Vector2 position, float width, float height)
{
	this->transform = GUITransform(position, width, height);
	this->color = Color();
}

GUI_Element::GUI_Element(GUITransform transform, Color color)
{
	this->transform = transform;
	this->color = color;
}
