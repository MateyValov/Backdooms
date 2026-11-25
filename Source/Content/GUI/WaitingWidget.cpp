#include "WaitingWidget.h"

#include "Architecture/GUI/GUI_Image.h"
#include "Architecture/GUI/GUI_Text.h"

WaitingWidget::WaitingWidget()
{
	elements.push_back(new GUI_Image("GUI/Square.png", GUITransform(Vector2(), Window::Fullscreen().x(), Window::Fullscreen().y()), Color(0.f, 0.f, 0.f, 0.3f)));
	elements.push_back(new GUI_Text("Waiting for everyone to finish...", GUITransform(Vector2(0.f, -50.f), 1.f, 1.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color()));
}
