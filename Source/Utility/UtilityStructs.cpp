#include "UtilityStructs.h"	


float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

float degToRad(float a)
{
	return a * PI / 180;
}

float FixAng(float a)
{
	if (a >= 360) { a -= 360; }
	if (a < 0) { a += 360; }
	return a;
}

Color Color::Red()
{
	return Color(1.f, 0.f, 0.f, 1.f);
}

Color Color::Green()
{
	return Color(0.f, 1.f, 0.f, 1.f);
}

Color Color::Blue()
{
	return Color(0.f, 0.f, 1.f, 1.f);
}
