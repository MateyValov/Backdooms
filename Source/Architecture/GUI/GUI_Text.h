#pragma once

#include "Architecture/GUI/GUI_Element.h"

#include <string>

using namespace std;

class GUI_Text : public GUI_Element
{
public:
	string text;

	GUI_Text(string text);
	GUI_Text(string text, Vector2 position);
	GUI_Text(string text, GUITransform transform, Color color);

	void Draw(Screenspace screenspace) override;
};