#pragma once

#include <vector>

#include "../Structs/Image.h"
#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"

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
	
};
