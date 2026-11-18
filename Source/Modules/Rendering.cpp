#include "Rendering.h"

#include "Textures.h"
#include "Fonts.h"

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
	const Fonts::Font& font = Fonts::GetBase();   // reference, not a copy
	if (!font.IsLoaded() || text.empty()) return;

	const Vector2 size = font.Measure(text, 1.0f);
	if (size.x <= 0.f || size.y <= 0.f) return;

	GUITransform box = transform;
	box.width = size.x;
	box.height = size.y;
	const GUITransform t = GetTrueGUITransform(box, localScreenspace);

	// Effective scale = transform.scale * viewport scale, already baked into t.width/t.height
	const float sx = t.width / size.x;
	const float sy = t.height / size.y;

	// Describe the font atlas as an Image so Draw() can use it
	Image atlas;
	atlas.texture = font.texture;
	atlas.width = Fonts::Font::AtlasSize;
	atlas.height = Fonts::Font::AtlasSize;

	float penX = 0.0f, lineTop = 0.0f;

	for (unsigned char c : text)
	{
		if (c == '\n')
		{
			penX = 0.0f;
			lineTop += font.lineHeight;
			continue;
		}

		stbtt_aligned_quad q;
		if (!font.GetGlyphQuad(c, penX, lineTop + font.ascent, q)) continue;

		const stbtt_bakedchar& g = font.glyphs[c - Fonts::Font::FirstChar];
		if (g.x1 <= g.x0) continue;

		Rect source((float)g.x0, (float)g.y0, (float)(g.x1 - g.x0), (float)(g.y1 - g.y0));
		Rect dest(t.position.x + q.x0 * sx,
			t.position.y + q.y0 * sy,
			(q.x1 - q.x0) * sx,
			(q.y1 - q.y0) * sy);

		Draw(atlas, dest, source, tint);
	}
}
