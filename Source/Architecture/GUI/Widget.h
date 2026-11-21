#pragma once

#include <vector>

#include "GUI_Element.h"

#include "../../Modules/Window.h"

using namespace std;

class Widget : public GUI_Element
{
protected:
	vector<GUI_Element*> elements;

public:
	virtual void Draw(Screenspace screenspace) override;
};
