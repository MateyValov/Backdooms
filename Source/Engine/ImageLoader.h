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