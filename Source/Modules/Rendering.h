#pragma once

#include "string"

#include "Structs/Image.h"
#include "Structs/Transform.h"
#include "Structs/GUITransform.h"

#include "Modules/Window.h"

using namespace std;

namespace Rendering
{
	void Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint = Color());
	void DrawTile(int index, const Rect& dest, const Color& tint = Color());
	void DrawImage(const Image& image, const GUITransform& transform, const Screenspace localScreenspace, const Color& tint = Color());
	void DrawString(const string& text, const GUITransform& transform, const Screenspace localScreenspace, const Color& tint = Color());
}
