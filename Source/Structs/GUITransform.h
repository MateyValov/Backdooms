#pragma once

#include "Transform.h"

#define TOP_LEFT      Vector2(-1.f,-1.f)
#define TOP_CENTER    Vector2( 0.f,-1.f)
#define TOP_RIGHT     Vector2( 1.f,-1.f)
#define MIDDLE_LEFT   Vector2(-1.f, 0.f)
#define MIDDLE_CENTER Vector2( 0.f, 0.f)
#define MIDDLE_RIGHT  Vector2( 1.f, 0.f)
#define BOTTOM_LEFT   Vector2(-1.f, 1.f)
#define BOTTOM_CENTER Vector2( 0.f, 1.f)
#define BOTTOM_RIGHT  Vector2( 1.f, 1.f)


struct GUITransform
{
	float width = 100;
	float height = 100;
	Vector2 position = Vector2();
	Vector2 scale = Vector2(1.f);
	Vector2 alignment = TOP_LEFT;
	Vector2 anchor = TOP_LEFT;

	GUITransform() = default;
	GUITransform(Vector2 position) : position(position) {}
	GUITransform(Vector2 position, float width, float height) : position(position), width(width), height(height) {}
	GUITransform(Vector2 position, float width, float height, Vector2 scale, Vector2 alignment, Vector2 anchor) : position(position), width(width), height(height), scale(scale), alignment(alignment), anchor(anchor) {}
};