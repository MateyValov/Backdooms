#include "Rendering.h"

#include "Textures.h"

void Rendering::Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint)
{
	if (image.texture == 0 || image.width <= 0 || image.height <= 0) { return; }

	float u0 = source.x / image.width, u1 = (source.x + source.w) / image.width;
	float v0 = source.y / image.height, v1 = (source.y + source.h) / image.height;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, image.texture);
	glColor4f(tint.r, tint.g, tint.b, tint.a);

	glBegin(GL_QUADS);
	glTexCoord2f(u0, v0); glVertex2f(dest.x, dest.y);
	glTexCoord2f(u1, v0); glVertex2f(dest.x + dest.w, dest.y);
	glTexCoord2f(u1, v1); glVertex2f(dest.x + dest.w, dest.y + dest.h);
	glTexCoord2f(u0, v1); glVertex2f(dest.x, dest.y + dest.h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Rendering::DrawImage(const Image& image, const Vector2& position, const Vector2& scale, const Color& tint)
{
	float w = (float)image.width * scale.x;
	float h = (float)image.height * scale.y;
	Draw(image, Rect(position.x, position.y, w, h), Rect(0, 0, (float)image.width, (float)image.height), tint);
}

void Rendering::DrawTile(int index, const Rect& dest, const Color& tint)
{
	Draw(Textures::Atlas::GetImage(), dest, Textures::Atlas::Cell(index), tint);
}

void Rendering::DrawString(const std::string& text, const Vector2& position, float scale, const Color& tint)
{
	//skip for now
}
