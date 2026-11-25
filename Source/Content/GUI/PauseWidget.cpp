#include "PauseWidget.h"

#include "Architecture/GUI/GUI_Image.h"
#include "Architecture/GUI/GUI_Text.h"

PauseWidget::PauseWidget()
{
	elements.push_back(new GUI_Image("GUI/Square.png", GUITransform(Vector2(), Window::Fullscreen().x(), Window::Fullscreen().y()), Color(0.f, 0.f, 0.f, 0.5f)));
	elements.push_back(new GUI_Text("PAUSE", GUITransform(Vector2(0.f, -150.f), 1.f, 1.f, Vector2(2.f), MIDDLE_CENTER, MIDDLE_CENTER), Color()));
	elements.push_back(new GUI_Text("Press 'Enter' to continue", GUITransform(Vector2(0.f, 50.f), 1.f, 1.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color()));
	elements.push_back(new GUI_Text("Press 'Escape' to quit", GUITransform(Vector2(0.f, 100.f), 1.f, 1.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color()));
	elements.push_back(new GUI_Text("Warning! If you quit, your score won't be saved", GUITransform(Vector2(0.f, -5.f), 1.f, 1.f, Vector2(0.5f), BOTTOM_CENTER, BOTTOM_CENTER), Color()));
}
