#include "GUI_Image.h"

#include "../../Modules/Rendering.h"
#include "../../Modules/Textures.h"


GUI_Image::GUI_Image(string path) : GUI_Element()
{
	this->path = path;
	this->scale = Vector2(1.f, 1.f);
	image = Textures::GUI::Get(path);
}

GUI_Image::GUI_Image(string path, Vector2 position) : GUI_Element(position)
{
	this->path = path;
	this->scale = Vector2(1.f, 1.f);
	image = Textures::GUI::Get(path);
}

GUI_Image::GUI_Image(string path, Vector2 position, Vector2 scale, Vector2 alignment, Color color) : GUI_Element(position, alignment, color)
{
	this->path = path;
	this->scale = scale;
	image = Textures::GUI::Get(path);
}

void GUI_Image::Draw(Vector2 TopLeft, Vector2 BottomRight)
{
	Rendering::DrawImage(image, GetTruePosition(TopLeft, BottomRight), scale, color);
}
