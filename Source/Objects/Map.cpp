#include "Map.h"

#include "Object.h"

#include <cfloat>
#include <cmath>
#include <algorithm>

static constexpr MapArray WALLS_BASE =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
	1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static constexpr MapArray FLOOR_BASE =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,0,3,0,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,
	0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,3,3,3,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

static constexpr MapArray ROOF_BASE =
{
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,0,4,0,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,4,4,5,4,0,
	0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,4,4,4,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

void Map::GenerateMap()
{
	ResetMap();
}

void Map::ResetMap()
{
	walls = WALLS_BASE;
	floor = FLOOR_BASE;
	roof = ROOF_BASE;
}

MapArray Map::GetWalls() const
{
	return walls;
}

MapArray Map::GetFloor() const
{
	return floor;
}

MapArray Map::GetRoof() const
{
	return roof;
}

bool Map::IsSolid(int tileX, int tileY) const
{
	if (tileX < 0 || tileY < 0 || tileX >= MAP_TILES_X || tileY >= MAP_TILES_Y) 
	{ 
		return true; 
	}
	return walls[tileY * MAP_TILES_X + tileX] > 0;
}

bool Map::OverlapsWall(const Vector2& centre, float radius) const
{
	int x0 = (int)floorf((centre.x - radius) / TILE_SIZE), x1 = (int)floorf((centre.x + radius) / TILE_SIZE);
	int y0 = (int)floorf((centre.y - radius) / TILE_SIZE), y1 = (int)floorf((centre.y + radius) / TILE_SIZE);
	for (int y = y0; y <= y1; ++y)
	{
		for (int x = x0; x <= x1; ++x)
		{
			if (IsSolid(x, y))
			{
				return true;
			}
		}
	}
		
	return false;
}

Vector2 Map::EvaluatePosition(Vector2 position, float radius)
{
	const float GAP = 0.01f; 

	position.x = std::min(std::max(position.x, radius), MAP_TILES_X * TILE_SIZE - radius);
	position.y = std::min(std::max(position.y, radius), MAP_TILES_Y * TILE_SIZE - radius);

	for (int iteration = 0; iteration < 32 && OverlapsWall(position, radius); ++iteration)
	{
		int x0 = (int)floorf((position.x - radius) / TILE_SIZE), x1 = (int)floorf((position.x + radius) / TILE_SIZE);
		int y0 = (int)floorf((position.y - radius) / TILE_SIZE), y1 = (int)floorf((position.y + radius) / TILE_SIZE);

		float bestAmount = FLT_MAX;
		Vector2 bestPos = position;
		bool any = false;
		auto consider = [&](float newX, float newY)
			{
				float amount = fabsf(newX - position.x) + fabsf(newY - position.y);
				if (amount < bestAmount) { bestAmount = amount; bestPos = Vector2(newX, newY); any = true; }
			};

		for (int ty = y0; ty <= y1; ++ty)
		{
			for (int tx = x0; tx <= x1; ++tx)
			{
				if (!IsSolid(tx, ty)) { continue; }
				float left = (float)(tx * TILE_SIZE), right = left + TILE_SIZE;
				float top = (float)(ty * TILE_SIZE), bottom = top + TILE_SIZE;

				if (!IsSolid(tx, ty - 1)) { consider(position.x, top - radius - GAP); }      
				if (!IsSolid(tx, ty + 1)) { consider(position.x, bottom + radius + GAP); }   
				if (!IsSolid(tx - 1, ty)) { consider(left - radius - GAP, position.y); }     
				if (!IsSolid(tx + 1, ty)) { consider(right + radius + GAP, position.y); }   
			}
		}
			
		if (any == false) 
		{ 
			break; 
		}
		position = bestPos;
	}
	if (OverlapsWall(position, radius) == false) 
	{ 
		return position; 
	}

	float bestDist = FLT_MAX;
	Vector2 best = position;

	for (int ty = 0; ty < MAP_TILES_Y; ++ty)
	{
		for (int tx = 0; tx < MAP_TILES_X; ++tx)
		{
			if (IsSolid(tx, ty)) 
			{ 
				continue; 
			}
			float minX = tx * TILE_SIZE + radius + GAP, maxX = (tx + 1) * TILE_SIZE - radius - GAP;
			float minY = ty * TILE_SIZE + radius + GAP, maxY = (ty + 1) * TILE_SIZE - radius - GAP;
			if (minX > maxX || minY > maxY) 
			{
				continue; 
			}   
			Vector2 c(std::min(std::max(position.x, minX), maxX), std::min(std::max(position.y, minY), maxY));
			float d = (c.x - position.x) * (c.x - position.x) + (c.y - position.y) * (c.y - position.y);
			if (d < bestDist) 
			{ 
				bestDist = d; 
				best = c; 
			}
		}
	}
		
	return best;
}

