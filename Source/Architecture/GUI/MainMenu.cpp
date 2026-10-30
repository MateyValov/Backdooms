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
}
