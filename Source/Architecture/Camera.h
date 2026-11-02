#pragma once

#include <vector>

#include "../Structs/Image.h"
#include "../Structs/MapArray.h"
#include "../Structs/Transform.h"
#include "../Structs/Screenspace.h"

using namespace std;

class Map;
class Object;

class Camera
{
private:
	Screenspace screenspace;

	int MaxRaySteps = 20;
	int fov = 60;
	int columnWidth = 4;
	float nearClippingDistance = 1.f;

	int columnCount;          
	float projDist;   

	vector<float> wallDepth;
	vector<unsigned char> floorBuf;   
	Image floorImage;                     

	vector<float> pxCos, pxSin, pxInvCos;
	vector<float> colCos, colSin;
	vector<float> pxDirX, pxDirY;

	void Setup();

	void DrawWalls(const MapArray walls, const Transform viewpoint);
	void DrawFloorAndRoof(const MapArray floor, const MapArray roof, const Transform viewpoint);
	void DrawObjects(const vector<Object*>& objects, const Transform viewpoint);

public:
	Camera();
	Camera(Screenspace screenspace, int fov, int maxRaySteps, int columnWidth, float nearClippingDistance);

	int GetWidth();
	int GetHeight();
	Screenspace GetScreenspace();

	void InitFloorBuffer();

	void DrawLevel(const Map* map, const Transform viewpoint);
	
};
