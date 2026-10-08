#pragma once

#define MAP_TILES_X 21
#define MAP_TILES_Y 21
#define TILE_SIZE 64

extern int walls[MAP_TILES_X * MAP_TILES_Y];
extern int ground[MAP_TILES_X * MAP_TILES_Y];
extern int ceiling[MAP_TILES_X * MAP_TILES_Y];

void GenerateMap();
void ResetMap();