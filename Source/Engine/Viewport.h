#pragma once

#include <string>
#include <vector>

#include "../Structs/Image.h"
#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"

#define RESOLUTION_WINDOW_X 640 
#define RESOLUTION_WINDOW_Y 480

#define MAX_RAY_STEPS 20           

#define FOV_DEGREES 60                                     
#define COLUMN_W 4      

using namespace std;

class Map;

class Viewport
{
private:
	Vector2 origin;

	int width;
	int height;

	int columnCount;          
	float projDist;   

	vector<unsigned char> floorBuf;   
	Image floorImage;                     

	vector<float> pxCos, pxSin, pxInvCos;
	vector<float> colCos, colSin;
	vector<float> pxDirX, pxDirY;

	void Setup();

	void DrawWalls(const MapArray walls, const Transform viewpoint);
	void DrawFloorAndRoof(const MapArray floor, const MapArray roof, const Transform viewpoint);

public:
	Viewport();
	Viewport(Vector2 origin, int width, int height);

	int GetWidth();
	int GetHeight();

	void InitFloorBuffer();

	void DrawLevel(const Map* map, const Transform viewpoint);

	void Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint = Color());
	void DrawImage(const Image& image, const Vector2& position, float widthOverride = 0, float heightOverride = 0, const Color& tint = Color());
	void DrawTile(int index, const Rect& dest, const Color& tint = Color());

	// skip for now
	void DrawString(const string& text, const Vector2& position, float scale = 1.f, const Color& tint = Color());
	
};
