#pragma once

#include <vector>

#include "../../Structs/Transform.h"

using namespace std;

class GUI_Element;

class Widget
{
protected:
	vector<GUI_Element*> elements;

public:
	void DrawWidget(Vector2 TopLeft, Vector2 BottomRight);
};
