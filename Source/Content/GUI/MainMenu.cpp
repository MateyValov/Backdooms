#include "MainMenu.h"

#include "Architecture/GUI/GUI_Image.h"
#include "Architecture/GUI/GUI_Text.h"

MainMenu::MainMenu()
{
	elements.push_back(new GUI_Image("GUI/TitleScreen.png", GUITransform(Vector2(), 640.f, 480.f), Color()));
	elements.push_back(new GUI_Image("GUI/Title.png", GUITransform(Vector2(0.f, 20.f), 400.f, 100.f, Vector2(1.f), TOP_CENTER, TOP_CENTER), Color()));
	elements.push_back(new GUI_Text("Press '1' for one player", GUITransform(Vector2(0.f, 50.f), 1.f, 1.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color::Black()));
	elements.push_back(new GUI_Text("Press '2' for two players", GUITransform(Vector2(0.f, 100.f), 1.f, 1.f, Vector2(1.f), MIDDLE_CENTER, MIDDLE_CENTER), Color::Black()));
}
