#include "Widget.h"

#include "GUI_Element.h"

void Widget::DrawWidget(Vector2 TopLeft, Vector2 BottomRight)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->Draw(TopLeft, BottomRight);
	}
}
