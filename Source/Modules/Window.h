#pragma once

#include "../Structs/Transform.h"

struct Screenspace
{
	Vector2 TopLeft;
	Vector2 BottomRight;

	Screenspace() : TopLeft(Vector2()), BottomRight(Vector2(640.f, 480.f)) {}
	Screenspace(Vector2 TopLeft, Vector2 BottomRight) : TopLeft(TopLeft), BottomRight(BottomRight) {}
	Screenspace(float width, float height) : TopLeft(Vector2()), BottomRight(Vector2(width, height)) {}
	Screenspace(Vector2 TopLeft, float width, float height) : TopLeft(TopLeft), BottomRight(Vector2(TopLeft.x + width, TopLeft.y + height)) {}

	float x() const
	{
		return BottomRight.x - TopLeft.x;
	}

	float y() const
	{
		return BottomRight.y - TopLeft.y;
	}

	float AspectRatio() const
	{
		return x() / y();
	}
};

class Window
{
private:
	inline static Screenspace fullscreen;

public:
	static void InitWindow(int width, int height);

	static Screenspace Fullscreen();

	static Screenspace TopHalf();
	static Screenspace BottomHalf();

	static Screenspace LeftHalf();
	static Screenspace RightHalf();

	static Screenspace TopLeft();
	static Screenspace TopRight();
	static Screenspace BottomLeft();
	static Screenspace BottomRight();
};