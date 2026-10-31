#pragma once

#include "Transform.h"


struct Screenspace
{
	Vector2 TopLeft;  
	Vector2 BottomRight; 

	Screenspace() : TopLeft(Vector2()), BottomRight(Vector2(640.f, 480.f)) {}
	Screenspace(Vector2 TopLeft, Vector2 BottomRight) : TopLeft(TopLeft), BottomRight(BottomRight) {}

	float x() const
	{
		return BottomRight.x - TopLeft.x;
	}

	float y() const
	{
		return BottomRight.y - TopLeft.y;
	}
};
