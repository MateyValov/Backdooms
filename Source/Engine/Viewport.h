#pragma once

#include <string>

#include "ImageLoader.h"

#define RESOLUTION_WINDOW_X 640 
#define RESOLUTION_WINDOW_Y 480

#define MAX_RAY_STEPS 20

#define BOTTOM_MARGIN 128                               // free space under the 3D view
#define VIEW_W RESOLUTION_WINDOW_X                         // 3D view is as wide as the window
#define VIEW_H (RESOLUTION_WINDOW_Y - BOTTOM_MARGIN)       // 412; keep this number even

#define FOV_DEGREES 60                                     // real field of view, in degrees
#define COLUMN_W 4                                         // pixels per ray; must divide VIEW_W exactly
#define RAY_COUNT (VIEW_W / COLUMN_W)                      // 256 rays
#define RAY_STEP ((float)FOV_DEGREES / RAY_COUNT)          // degrees between neighbouring rays

#define PROJ_DIST VIEW_H   // how tall walls look: a wall one tile away fills the view height

using namespace std;

class Map;

class Viewport
{
private:
	Vector2 origin;

	int width;
	int height;

	unsigned char floorBuf[VIEW_W * VIEW_H * 4];
	GLuint floorBufTex;

public:
	Viewport();
	Viewport(Vector2 origin, int width, int height);

	int GetWidth();
	int GetHeight();

	void DrawLevel(Map* map, Transform viewpoint);
	void Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint = Color());
	void DrawImage(const Image& image, const Vector2& position, float widthOverride = 0, float heightOverride = 0, const Color& tint = Color());
	void DrawTile(const Tileset& tileset, int index, const Rect& dest, const Color& tint = Color());
	void DrawString(const string& text, const Vector2& position, float scale = 1.f, const Color& tint = Color());
	
};
