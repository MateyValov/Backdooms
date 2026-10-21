#pragma once

#include <array>

#include "../Utility/UtilityStructs.h"

#define MAP_TILES_X 21
#define MAP_TILES_Y 21
#define TILE_SIZE 64

using namespace std;
using MapArray = array<int, MAP_TILES_X* MAP_TILES_Y>;

class Map
{
private:
	MapArray walls;
	MapArray floor;
	MapArray roof;

public:
	void GenerateMap();
	void ResetMap();

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;

	bool IsSolid(int tileX, int tileY) const;
	bool OverlapsWall(const Vector2& centre, float radius) const;
	Vector2 MoveAndCollide(Vector2 position, Vector2 delta, float radius) const;
};