#include "MainMenu.h"

#include "../../Architecture/GUI/GUI_Image.h"

MainMenu::MainMenu()
{
	elements.push_back(new GUI_Image(
		"GUI/TitleScreen.png"
	));
}
