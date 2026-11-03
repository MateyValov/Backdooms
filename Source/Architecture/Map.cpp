#include "Map.h"

#include "Object.h"
#include "Player.h"

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <unordered_set>


static float WallTouchMargin = 2.f;

static MapArray WALLS_BASE =
{
	1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,
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

static MapArray FLOOR_BASE =
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

static MapArray ROOF_BASE =
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

Map::Map()
{
	GenerateMap();
}

void Map::GenerateMap()
{
	ResetMap();
}

void Map::ResetMap()
{
	walls = WALLS_BASE;
	floor = FLOOR_BASE;
	roof = ROOF_BASE;
	DestroyAllObjects();
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

Vector2 Map::GetSpawnpoint() const
{
	return Vector2(Vector2((MAP_TILES_X / 2.f) * TILE_SIZE, (MAP_TILES_Y / 2.f) * TILE_SIZE));
}

int Map::GetTile(int x, int y) const
{
	return walls[y * MAP_TILES_X + x];
}

vector<Object*> Map::GetObjects() const
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

bool Map::IsMarked(const Object* object) const
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

void Map::RunCollisions()
{
	// Who takes part: objects with collision that are not marked for destruction. This is a snapshot
	// of plain pointers, so callbacks may Spawn or Destroy freely; new objects join next frame.
	vector<Object*> active;
	for (const ObjectEntry& entry : objects)
	{
		if (entry.destroyed == false && entry.object->HasCollision()) 
		{ 
			active.push_back(entry.object.get()); 
		}
	}

	// ======================= object vs object (circles) =======================
	auto makePair = [](Object* a, Object* b) { return less<Object*>()(a, b) ? ObjectPair(a, b) : ObjectPair(b, a); };

	set<ObjectPair, PairLess> now;
	for (size_t i = 0; i < active.size(); ++i)
	{
		for (size_t j = i + 1; j < active.size(); ++j)
		{
			Vector2 pa = active[i]->GetTransform().position, pb = active[j]->GetTransform().position;
			float dx = pb.x - pa.x, dy = pb.y - pa.y;
			float reach = active[i]->GetRadius() + active[j]->GetRadius();
			if (dx * dx + dy * dy < reach * reach) { now.insert(makePair(active[i], active[j])); }
		}
	}

	vector<ObjectPair> entered, stayed, exited;
	set_difference(now.begin(), now.end(), touchingPairs.begin(), touchingPairs.end(), back_inserter(entered), PairLess());
	set_intersection(now.begin(), now.end(), touchingPairs.begin(), touchingPairs.end(), back_inserter(stayed), PairLess());
	set_difference(touchingPairs.begin(), touchingPairs.end(), now.begin(), now.end(), back_inserter(exited), PairLess());
	touchingPairs = now;   // the new state is in place before any callback runs

	// Each event goes to both objects, unless one of them was destroyed by an earlier callback.
	auto deliver = [this](const ObjectPair& pair, void (Object::* event)(Object*))
		{
			if (!IsMarked(pair.first)) { (pair.first->*event)(pair.second); }
			if (!IsMarked(pair.second)) { (pair.second->*event)(pair.first); }
		};
	for (const ObjectPair& pair : exited) { deliver(pair, &Object::OnCollisionExit); }
	for (const ObjectPair& pair : entered) { deliver(pair, &Object::OnCollisionEnter); }
	for (const ObjectPair& pair : stayed) { deliver(pair, &Object::OnCollisionStay); }

	// ======================= object vs wall tiles =======================
	const MapArray& wallValues = GetWalls();
	const float T = (float)TILE_SIZE;

	for (Object* object : active)
	{
		if (IsMarked(object)) { continue; }   // destroyed by a callback a moment ago

		Vector2 p = object->GetTransform().position;
		float reach = object->GetRadius() + WallTouchMargin;
		int x0 = (int)floorf((p.x - reach) / T), x1 = (int)floorf((p.x + reach) / T);
		int y0 = (int)floorf((p.y - reach) / T), y1 = (int)floorf((p.y + reach) / T);

		vector<Tile> current;       // sorted by tile index: the loops run in that order
		for (int ty = y0; ty <= y1; ++ty)
		{
			for (int tx = x0; tx <= x1; ++tx)
			{
				if (tx < 0 || ty < 0 || tx >= MAP_TILES_X || ty >= MAP_TILES_Y) { continue; }   // off the map: not reported
				if (IsSolid(tx, ty)) { current.push_back(Tile{ tx, ty, wallValues[ty * MAP_TILES_X + tx] }); }
			}
		}

		vector<Tile>& previous = touchingWalls[object];
		vector<Tile> wallsEntered, wallsStayed, wallsExited;
		size_t i = 0, j = 0;
		while (i < current.size() || j < previous.size())
		{
			if (j >= previous.size() || (i < current.size() && current[i].tileIndex() < previous[j].tileIndex())) 
			{ 
				wallsEntered.push_back(current[i++]); 
			}
			else if (i >= current.size() || previous[j].tileIndex() < current[i].tileIndex()) 
			{ 
				wallsExited.push_back(previous[j++]);
			}
			else 
			{ 
				wallsStayed.push_back(current[i]); 
				i++; 
				j++; 
			}
		}
		previous = current;

		for (const Tile& w : wallsExited) { if (!IsMarked(object)) { object->OnWallExit(w.x, w.y, w.value); } }
		for (const Tile& w : wallsEntered) { if (!IsMarked(object)) { object->OnWallEnter(w.x, w.y, w.value); } }
		for (const Tile& w : wallsStayed) { if (!IsMarked(object)) { object->OnWallStay(w.x, w.y, w.value); } }
	}

	// Objects that stopped taking part (their collision was switched off) let go of their wall tiles.
	unordered_set<Object*> activeSet(active.begin(), active.end());
	vector<pair<Object*, std::vector<Tile>>> letGo;
	for (auto it = touchingWalls.begin(); it != touchingWalls.end(); )
	{
		if (activeSet.count(it->first) || IsMarked(it->first)) { ++it; continue; }
		letGo.push_back({ it->first, std::move(it->second) });
		it = touchingWalls.erase(it);
	}
	for (auto& entry : letGo)
	{
		for (const Tile& w : entry.second)
		{
			if (!IsMarked(entry.first)) { entry.first->OnWallExit(w.x, w.y, w.value); }
		}
	}
}

void Map::RemoveDestroyed()
{
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		vector<ObjectPair> goodbyes;     // (survivor, leaving)
		for (auto it = touchingPairs.begin(); it != touchingPairs.end(); )
		{
			bool firstGone = IsMarked(it->first), secondGone = IsMarked(it->second);
			if (!firstGone && !secondGone) { ++it; continue; }
			if (!firstGone) { goodbyes.push_back(ObjectPair(it->first, it->second)); }
			else if (!secondGone) { goodbyes.push_back(ObjectPair(it->second, it->first)); }
			it = touchingPairs.erase(it);
		}
		for (const ObjectPair& goodbye : goodbyes)
		{
			if (!IsMarked(goodbye.first)) 
			{ 
				goodbye.first->OnCollisionExit(goodbye.second); 
			}
			repeat = true;
		}
	}

	for (auto it = touchingWalls.begin(); it != touchingWalls.end(); )
	{
		if (IsMarked(it->first)) { it = touchingWalls.erase(it); }
		else { ++it; }
	}

	objects.erase(std::remove_if(objects.begin(), objects.end(),
		[](const ObjectEntry& entry) { return entry.destroyed; }), objects.end());
}

Vector2 Map::EvaluatePosition(Vector2 position, float radius)
{
	const float GAP = 0.01f; 

	position.x = min(max(position.x, radius), MAP_TILES_X * TILE_SIZE - radius);
	position.y = min(max(position.y, radius), MAP_TILES_Y * TILE_SIZE - radius);

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

void Map::UpdateObjects()
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

	RunCollisions();
	RemoveDestroyed();
}

void Map::Destroy(Object* object)
{
	if (!object) { return; }

	for (ObjectEntry& entry : objects)
	{
		if (entry.object.get() == object) 
		{ 
			entry.destroyed = true; 
			return; 
		}
	}
}

void Map::DestroyAllObjects()
{
	for (ObjectEntry& entry : objects) 
	{ 
		entry.destroyed = true;	
	}
}

int Map::ObjectCount() const
{
	int n = 0;
	for (const ObjectEntry& entry : objects) 
	{ 
		if (!entry.destroyed) { n++; } 
	}
	return n;
}

