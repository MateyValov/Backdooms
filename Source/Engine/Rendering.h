#pragma once

#include "string"

#include "../Structs/Image.h"
#include "../Structs/Transform.h"

using namespace std;

namespace Rendering
{
	void Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint = Color());
	void DrawImage(const Image& image, const Vector2& position, float widthOverride = 0, float heightOverride = 0, const Color& tint = Color());
	void DrawTile(int index, const Rect& dest, const Color& tint = Color());
	void DrawString(const string& text, const Vector2& position, float scale = 1.f, const Color& tint = Color());
}
