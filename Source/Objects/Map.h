#pragma once

#include <vector>

#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"

class Object;

class Map
{
private:
	MapArray walls;
	MapArray floor;
	MapArray roof;

	vector<Object*> objects;

	bool IsSolid(int tileX, int tileY) const;
	bool OverlapsWall(const Vector2& centre, float radius) const;

public:
	void GenerateMap();
	void ResetMap();

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;

	Vector2 EvaluatePosition(Vector2 position, float radius);
};