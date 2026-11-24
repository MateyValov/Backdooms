#include "GameOverWidget.h"

#include "Architecture/GUI/GUI_Image.h"
#include "Architecture/GUI/GUI_Text.h"

GameOverWidget::GameOverWidget()
{
	elements.push_back(new GUI_Image("GUI/Square.png", GUITransform(Vector2(), Window::Fullscreen().x(), Window::Fullscreen().y()), Color(0.3f, 0.3f, 0.3f)));
	elements.push_back(new GUI_Text("YOU DIED!", GUITransform(Vector2(0.f, -150.f), 1.f, 1.f, Vector2(2.f), MIDDLE_CENTER, MIDDLE_CENTER), Color::Red()));
}
