#include "GUI_Text.h"

#include "Modules/Rendering.h"

GUI_Text::GUI_Text(string text)
{
	this->text = text;
}

GUI_Text::GUI_Text(string text, Vector2 position) : GUI_Element(position)
{
	this->text = text;
}

GUI_Text::GUI_Text(string text, GUITransform transform, Color color) : GUI_Element(transform, color)
{
	this->text = text;
}

void GUI_Text::Draw(Screenspace screenspace)
{
	Rendering::DrawString(text, transform, screenspace, color);
}
