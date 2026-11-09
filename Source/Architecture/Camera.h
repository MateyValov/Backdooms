#pragma once

#include "Object.h"

#include <vector>

#include "../Modules/Window.h"

#include "../Structs/Image.h"
#include "../Structs/MapArray.h"
#include "../Structs/Transform.h"

using namespace std;

class Camera : public Object
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

	void DrawWalls(const MapArray walls);
	void DrawFloorAndRoof(const MapArray floor, const MapArray roof);
	void DrawObjects(const vector<Object*>& objects);

protected:
	void OnStart() override;
	void OnUpdate() override;

public:
	Camera();
	Camera(Screenspace screenspace, int fov, int maxRaySteps, int columnWidth, float nearClippingDistance);

	int GetWidth();
	int GetHeight();
	Screenspace GetScreenspace();

	void InitFloorBuffer();
	
};
