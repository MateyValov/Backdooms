#pragma once
#include <array>

#define MAP_TILES_X 21
#define MAP_TILES_Y 21
#define TILE_SIZE 64

using namespace std;
using MapArray = array<int, MAP_TILES_X* MAP_TILES_Y>;

struct Tile
{
	int x;
	int y;
	int value;

	int tileIndex() const
	{
		return y * MAP_TILES_X + x;
	}
};