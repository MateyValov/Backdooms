#include "Widget.h"

void Widget::Draw(Screenspace screenspace)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->Draw(screenspace);
	}
}
