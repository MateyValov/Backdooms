#pragma once

#include <math.h>

#define PI 3.141593


static float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

static float degToRad(float a)
{
	return a * PI / 180;
}

static float clampAngle(float a)
{
	if (a >= 360) { a -= 360; }
	if (a < 0) { a += 360; }
	return a;
}

static float clamp(float n, float min, float max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

static int clamp(int n, int min, int max)
{
	if (n < min) return min;
	if (n > max) return max;
	return n;
}

