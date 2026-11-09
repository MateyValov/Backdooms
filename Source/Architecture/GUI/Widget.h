#pragma once

#include <vector>

#include "../../Structs/Transform.h"

#include "../../Modules/Window.h"

using namespace std;

class GUI_Element;

class Widget
{
protected:
	vector<GUI_Element*> elements;

public:
	void DrawWidget(Screenspace screenspace);
};
