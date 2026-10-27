#pragma once
#include <GL/glut.h>
#include <vector>

#include "../Structs/Transform.h"
#include "../Structs/Image.h"

using namespace std;

//extern vector<unsigned char> tilesetPixels;   // RGBA, top row first
//
//// Tileset state, filled in by loadTileset().
//// The tileset is a vertical stack of square tiles: tile size = image width,
//// tile count = image height / image width.
//extern GLuint tilesetTex;   // OpenGL texture handle
//extern int texW;            // image width in pixels (= tile size)
//extern int texH;            // image height in pixels
//extern int tileCount;       // number of tiles stacked in the image

bool loadImage(const char* path, Image& out, bool keepPixels = false);
bool loadTileset(const char* path, int tileCount, Tileset& out, bool keepPixels = false);

class ImageLoader
{
private:
	static Tileset TerrainTileset;

public:

};