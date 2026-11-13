#include "Rendering.h"

#include "Textures.h"

static Vector2 GetLocalScreenspaceScale(Screenspace localScreenspace)
{
	Screenspace globalScreenspace = Window::Fullscreen();
	return Vector2(localScreenspace.x() / globalScreenspace.x(), localScreenspace.y() / globalScreenspace.y());
}

static GUITransform GetTrueGUITransform(GUITransform transform, Screenspace localScreenspace)
{
	Screenspace globalScreenspace = Window::Fullscreen();
	const Vector2 vs = Vector2(localScreenspace.x() / globalScreenspace.x(), localScreenspace.y() / globalScreenspace.y());

	const float halfW = localScreenspace.x() * 0.5f;
	const float halfH = localScreenspace.y() * 0.5f;
	const float centerX = localScreenspace.TopLeft.x + halfW;
	const float centerY = localScreenspace.TopLeft.y + halfH;

	const float originX = centerX + transform.alignment.x * halfW + transform.position.x * vs.x;
	const float originY = centerY + transform.alignment.y * halfH + transform.position.y * vs.y;

	const float w = transform.width * transform.scale.x * vs.x;
	const float h = transform.height * transform.scale.y * vs.y;
	const Vector2 trueOrigin = Vector2(originX - (transform.anchor.x + 1.f) * 0.5f * w, originY - (transform.anchor.y + 1.f) * 0.5f * h);

	return GUITransform(trueOrigin, w, h);
}

void Rendering::Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint)
{
	if (image.texture == 0 || image.width <= 0 || image.height <= 0) 
	{ 
		return; 
	}

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

void Rendering::DrawTile(int index, const Rect& dest, const Color& tint)
{
	Draw(Textures::Atlas::GetImage(), dest, Textures::Atlas::Cell(index), tint);
}

void Rendering::DrawImage(const Image& image, const GUITransform& transform, const Screenspace localScreenspace, const Color& tint)
{
	GUITransform trueTransform = GetTrueGUITransform(transform, localScreenspace);

	float x = trueTransform.position.x;
	float y = trueTransform.position.y;
	float w = trueTransform.width;
	float h = trueTransform.height;

	Draw(image, Rect(x, y, w, h), Rect(0, 0, (float)image.width, (float)image.height), tint);
}

void Rendering::DrawString(const string& text, const GUITransform& transform, const Screenspace localScreenspace, const Color& tint)
{
	//to be added someday
}
