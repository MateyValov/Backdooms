#pragma once
#include <GL/glut.h>

#include <vector>

using namespace std;

extern vector<unsigned char> tilesetPixels;   // RGBA, top row first

// Tileset state, filled in by loadTileset().
// The tileset is a vertical stack of square tiles: tile size = image width,
// tile count = image height / image width.
extern GLuint tilesetTex;   // OpenGL texture handle
extern int texW;            // image width in pixels (= tile size)
extern int texH;            // image height in pixels
extern int tileCount;       // number of tiles stacked in the image

bool loadTileset();

struct Image
{
	GLuint texture = 0;
	int width = 0;
	int height = 0;
};

// Loads any BMP/PNG/TGA/JPG into an OpenGL texture. Needs an active OpenGL
// context (call after glutCreateWindow). The path is relative to the working
// directory, e.g. "Textures/Gui.bmp". Returns false and prints why on failure.
bool loadImage(const char* path, Image& out);

// Draws the image as a rectangle at (x, y) with the given size, in the same
// coordinates as the rest of your 2D drawing. Transparent pixels (alpha) are
// respected for formats that have alpha, such as PNG and TGA.
void drawImage(const Image& img, float x, float y, float w, float h);