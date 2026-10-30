#pragma once

#include <vector>

#include "../Structs/Image.h"
#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"
    

using namespace std;

class Map;

class Viewport
{
private:
	Vector2 origin;

	int width;
	int height;

	int MaxRaySteps = 20;
	int fov = 60;
	int columnWidth = 4;

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
	Viewport(Vector2 origin, int width, int height, int fov, int maxRaySteps, int columnWidth);

	int GetWidth();
	int GetHeight();

	void InitFloorBuffer();

	void DrawLevel(const Map* map, const Transform viewpoint);
	
};
