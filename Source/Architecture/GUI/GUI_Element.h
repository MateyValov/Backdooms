#pragma once

#include "../../Structs/Transform.h"
#include "../../Structs/Image.h"

#define TOP_LEFT      Vector2(-1.f,-1.f)
#define TOP_CENTER    Vector2( 0.f,-1.f)
#define TOP_RIGHT     Vector2( 1.f,-1.f)
#define MIDDLE_LEFT   Vector2(-1.f, 0.f)
#define MIDDLE_CENTER Vector2( 0.f, 0.f)
#define MIDDLE_RIGHT  Vector2( 1.f, 0.f)
#define BOTTON_LEFT   Vector2(-1.f, 1.f)
#define BOTTON_CENTER Vector2( 0.f, 1.f)
#define BOTTON_RIGHT  Vector2( 1.f, 1.f)

class GUI_Element
{
public:
	Vector2 position;
	Vector2 alignment = TOP_LEFT;
	Color color;

	GUI_Element();
	GUI_Element(Vector2 position);
	GUI_Element(Vector2 position, Vector2 alignment, Color color);

	Vector2 GetTruePosition(Vector2 TopLeft, Vector2 BottomRight);

	virtual void Draw(Vector2 TopLeft, Vector2 BottomRight) = 0;
};