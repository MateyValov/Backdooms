#pragma once
#include <GL/glut.h>
#include <vector>

struct Rect
{
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;

	Rect() = default;
	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
};

struct Image
{
	GLuint texture = 0;
	int width = 0;
	int height = 0;
	std::vector<unsigned char> pixels;
};

struct Tileset
{
	Image image;
	int tileCount = 1;
	int tileSize = 0;
};

struct Color
{
	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;

	Color() = default;
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	static Color Red()
	{
		return Color(1.f, 0.f, 0.f, 1.f);
	}

	static Color Green()
	{
		return Color(0.f, 1.f, 0.f, 1.f);
	}

	static Color Blue()
	{
		return Color(0.f, 0.f, 1.f, 1.f);
	}
};
