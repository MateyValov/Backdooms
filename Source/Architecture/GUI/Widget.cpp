#include "Widget.h"

#include "GUI_Element.h"

void Widget::DrawWidget(Screenspace screenspace)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->Draw(screenspace);
	}
}
