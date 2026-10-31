#include "MainMenu.h"

#include "GUI_Image.h"

MainMenu::MainMenu()
{
	elements.push_back(new GUI_Image(
		"GUI/TitleScreen.png"
	));
	elements.push_back(new GUI_Image(
		"GUI/Bar.png", Vector2(120, 240)
	));
	elements.push_back(new GUI_Image(
		"GUI/maznite_sword.png", Vector2(-120, -240), Vector2(1.5f, 0.8f), BOTTON_RIGHT, Color::Red()
	));
	elements.push_back(new GUI_Image(
		"GUI/maznite_sword.png", Vector2(0.f, 0.f), Vector2(10.f, 10.f), TOP_LEFT, Color()
	));
}
