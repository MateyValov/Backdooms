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
	Vector2 scale;

	GUI_Image(string path);
	GUI_Image(string path, Vector2 position);
	GUI_Image(string path, Vector2 position, Vector2 scale, Vector2 alignment, Color color);

	void Draw(Vector2 TopLeft, Vector2 BottomRight) override;
};