#include "GUI_Image.h"

#include "../../Modules/Rendering.h"
#include "../../Modules/Textures.h"


GUI_Image::GUI_Image(string path) : GUI_Element()
{
	this->path = path;
	image = Textures::GUI::Get(path);
}

GUI_Image::GUI_Image(string path, Vector2 position, float width, float height) : GUI_Element(position, width, height)
{
	this->path = path;
	image = Textures::GUI::Get(path);
}

GUI_Image::GUI_Image(string path, GUITransform transform, Color color) : GUI_Element(transform, color)
{
	this->path = path;
	image = Textures::GUI::Get(path);
}

void GUI_Image::Draw(Screenspace screenspace)
{
	Rendering::DrawImage(image, transform, screenspace, color);
}
