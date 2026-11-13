#pragma once

#include <string>

#include "GUI_Element.h"

using namespace std;

class GUI_Image : public GUI_Element
{
protected:
	string path;
	Image image;

public:
	GUI_Image(string path);
	GUI_Image(string path, Vector2 position, float width, float height);
	GUI_Image(string path, GUITransform transform, Color color);

	void Draw(Screenspace screenspace) override;
};