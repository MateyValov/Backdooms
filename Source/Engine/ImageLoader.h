#pragma once
#include <GL/glut.h>
#include <vector>

#include "../Utility/UtilityStructs.h"

using namespace std;

struct Image
{
	GLuint texture = 0;
	int width = 0;
	int height = 0;
};

struct Tileset
{
	Image image;
	int tileCount = 1;
	int tileW = 0;
	int tileH = 0;
};

extern vector<unsigned char> tilesetPixels;   // RGBA, top row first

// Tileset state, filled in by loadTileset().
// The tileset is a vertical stack of square tiles: tile size = image width,
// tile count = image height / image width.
extern GLuint tilesetTex;   // OpenGL texture handle
extern int texW;            // image width in pixels (= tile size)
extern int texH;            // image height in pixels
extern int tileCount;       // number of tiles stacked in the image

class ImageLoader
{
private:
	Tileset TerrainTileset;
	Tileset ItemTileset;
	Tileset MonsterTileset;

public:
	bool loadTileset();
	bool loadTileset(const char* path, int tileCount, Tileset& out);
	bool loadImage(const char* path, Image& out);

	void drawImage(const Image& image, Vector2 position, float widthOverride = 0, float heightOverride = 0, Color tint = Color());
	void drawNumber(int value, Vector2 position, Color tint = Color());
};