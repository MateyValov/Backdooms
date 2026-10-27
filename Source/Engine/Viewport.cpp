#include "Viewport.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

Viewport::Viewport()
{
	origin = Vector2();
	width = RESOLUTION_WINDOW_X;
	height = RESOLUTION_WINDOW_Y;
}

Viewport::Viewport(Vector2 origin, int width, int height)
{
	this->origin = origin;
	this->width = width;
	this->height = height;
}

int Viewport::GetWidth()
{
	return width;
}

int Viewport::GetHeight()
{
	return height;
}

void Viewport::DrawLevel(Map* map, Transform viewpoint)
{
}

void Viewport::Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint)
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

void Viewport::DrawImage(const Image& image, const Vector2& position, float widthOverride, float heightOverride, const Color& tint)
{
	float w = widthOverride > 0 ? widthOverride : (float)image.width;
	float h = heightOverride > 0 ? heightOverride : (float)image.height;
	Draw(image, Rect(position.x, position.y, w, h), Rect(0, 0, (float)image.width, (float)image.height), tint);
}

void Viewport::DrawTile(const Tileset& tileset, int index, const Rect& dest, const Color& tint)
{
	if (tileset.tileCount < 1) { return; }
	index = ((index % tileset.tileCount) + tileset.tileCount) % tileset.tileCount;
	Draw(tileset.image, dest, Rect(0, (float)(index * tileset.tileH), (float)tileset.tileW, (float)tileset.tileH), tint);
}

void Viewport::DrawString(const std::string& text, const Vector2& position, float scale, const Color& tint)
{
	//to be continued
}


