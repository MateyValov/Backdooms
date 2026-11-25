#include "Level.h"

#include "Modules/Physics.h"
#include "Architecture/Object.h"
#include "Architecture/GameState.h"
#include "Components/Collider.h"

#include <cmath>
#include <cfloat>
#include <iterator>
#include <algorithm>

void Level::Unregister(Object* object)
{
	cameras.erase(std::remove_if(cameras.begin(), cameras.end(),
		[object](Camera* c) { return static_cast<Object*>(c) == object; }), cameras.end());
	viewports.erase(std::remove_if(viewports.begin(), viewports.end(),
		[object](Viewport* v) { return static_cast<Object*>(v) == object; }), viewports.end());
}

Viewport* Level::GetGlobalViewport()
{
	return globalViewport;
}

GameState* Level::GetGameState()
{
	return gameState;
}

MapArray Level::GetWalls() const
{
	return walls;
}

MapArray Level::GetFloor() const
{
	return floor;
}

MapArray Level::GetRoof() const
{
	return roof;
}

int Level::GetTile(int x, int y) const
{
	return walls[y * MAP_TILES_X + x];
}

vector<Object*> Level::GetObjects() const
{
	vector<Object*> existing;
	existing.reserve(objects.size());
	for (const ObjectEntry& entry : objects)
	{
		if (entry.destroyed) 
		{ 
			continue;
		}
		existing.push_back(entry.object.get());
	}
	return existing;
}

void Level::SetWallTile(int x, int y, int value)
{
	walls[y * MAP_TILES_X + x] = value;
}

void Level::SetFloorTile(int x, int y, int value)
{
	floor[y * MAP_TILES_X + x] = value;
}

void Level::SetRoofTile(int x, int y, int value)
{
	roof[y * MAP_TILES_X + x] = value;
}

bool Level::IsSolid(int tileX, int tileY) const
{
	if (tileX < 0 || tileY < 0 || tileX >= MAP_TILES_X || tileY >= MAP_TILES_Y) 
	{ 
		return true; 
	}
	return walls[tileY * MAP_TILES_X + tileX] > 0;
}

bool Level::OverlapsWall(const Vector2& centre, float radius) const
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

bool Level::IsMarked(const Object* object) const
{
	for (const ObjectEntry& entry : objects)
	{
		if (entry.object.get() == object) 
		{ 
			return entry.destroyed; 
		}
	}
	return true;
}

void Level::RemoveDestroyed()
{
	Physics::ReleaseDestroyed(this);
	objects.erase(std::remove_if(objects.begin(), objects.end(),
		[](const ObjectEntry& entry) { return entry.destroyed; }), objects.end());
}

Vector2 Level::EvaluatePosition(Object* obj, Vector2 newPosition)
{
	Collider* collider = obj->GetComponent<Collider>();
	if (collider == nullptr)
	{
		return newPosition;
	}

	const float GAP = 0.01f;
	const float radius = collider->GetRadius();

	newPosition.x = min(max(newPosition.x, radius), MAP_TILES_X * TILE_SIZE - radius);
	newPosition.y = min(max(newPosition.y, radius), MAP_TILES_Y * TILE_SIZE - radius);

	for (int iteration = 0; iteration < 32 && OverlapsWall(newPosition, radius); ++iteration)
	{
		int x0 = (int)floorf((newPosition.x - radius) / TILE_SIZE), x1 = (int)floorf((newPosition.x + radius) / TILE_SIZE);
		int y0 = (int)floorf((newPosition.y - radius) / TILE_SIZE), y1 = (int)floorf((newPosition.y + radius) / TILE_SIZE);

		float bestAmount = FLT_MAX;
		Vector2 bestPos = newPosition;
		bool any = false;
		auto consider = [&](float newX, float newY)
			{
				float amount = fabsf(newX - newPosition.x) + fabsf(newY - newPosition.y);
				if (amount < bestAmount) { bestAmount = amount; bestPos = Vector2(newX, newY); any = true; }
			};

		for (int ty = y0; ty <= y1; ++ty)
		{
			for (int tx = x0; tx <= x1; ++tx)
			{
				if (!IsSolid(tx, ty)) { continue; }
				float left = (float)(tx * TILE_SIZE), right = left + TILE_SIZE;
				float top = (float)(ty * TILE_SIZE), bottom = top + TILE_SIZE;

				if (!IsSolid(tx, ty - 1)) { consider(newPosition.x, top - radius - GAP); }
				if (!IsSolid(tx, ty + 1)) { consider(newPosition.x, bottom + radius + GAP); }
				if (!IsSolid(tx - 1, ty)) { consider(left - radius - GAP, newPosition.y); }
				if (!IsSolid(tx + 1, ty)) { consider(right + radius + GAP, newPosition.y); }
			}
		}
			
		if (any == false) 
		{ 
			break; 
		}
		newPosition = bestPos;
	}
	if (OverlapsWall(newPosition, radius) == false)
	{ 
		return newPosition;
	}

	float bestDist = FLT_MAX;
	Vector2 best = newPosition;

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
			Vector2 c(min(max(newPosition.x, minX), maxX), min(max(newPosition.y, minY), maxY));
			float d = (c.x - newPosition.x) * (c.x - newPosition.x) + (c.y - newPosition.y) * (c.y - newPosition.y);
			if (d < bestDist) 
			{ 
				bestDist = d; 
				best = c; 
			}
		}
	}
		
	return best;
}

void Level::UpdateObjects()
{
	size_t count = objects.size();
	for (size_t i = 0; i < count; ++i)
	{
		if (objects[i].destroyed) 
		{
			continue;
		}
		objects[i].object->Update();
	}
}

void Level::Destroy(Object* object)
{
	if (!object) { return; }

	for (ObjectEntry& entry : objects)
	{
		if (entry.object.get() == object) 
		{ 
			if (entry.destroyed) { return; }
			entry.destroyed = true; 
			Unregister(object);
			object->OnDestroy();
			object->DetachFromHierarchy();
			return; 
		}
	}
}

int Level::ObjectCount() const
{
	int n = 0;
	for (const ObjectEntry& entry : objects) 
	{ 
		if (!entry.destroyed) { n++; } 
	}
	return n;
}

vector<Camera*> Level::GetCameras() const
{
	return cameras;
}

vector<Viewport*> Level::GetViewports() const
{
	vector<Viewport*> sorted = viewports;
	stable_sort(sorted.begin(), sorted.end(),
		[](const Viewport* a, const Viewport* b) { return a->GetDrawOrder() < b->GetDrawOrder(); });
	return sorted;
}

