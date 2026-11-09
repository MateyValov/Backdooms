#include "Level.h"

#include "Object.h"
#include "Components/Collider.h"

#include <cfloat>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <unordered_set>


static float WallTouchMargin = 1.f;

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

Level::Level()
{
	GenerateLevel();
}

void Level::GenerateLevel()
{
	ResetLevel();
}

void Level::ResetLevel()
{
	walls = WALLS_BASE;
	floor = FLOOR_BASE;
	roof = ROOF_BASE;
	DestroyAllObjects();
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

Vector2 Level::GetSpawnpoint() const
{
	return Vector2(Vector2((MAP_TILES_X / 2.f) * TILE_SIZE, (MAP_TILES_Y / 2.f) * TILE_SIZE));
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

void Level::RunCollisions()
{
	struct Participant { Object* object; Collider* collider; };
	vector<Participant> active;
	for (const ObjectEntry& entry : objects)
	{
		if (entry.destroyed) { continue; }
		Collider* collider = entry.object->GetComponent<Collider>();
		if (collider == nullptr || !collider->IsActive()) { continue; }
		active.push_back(Participant{ entry.object.get(), collider });
	}

	auto makePair = [](Object* a, Object* b) { return less<Object*>()(a, b) ? ObjectPair(a, b) : ObjectPair(b, a); };

	set<ObjectPair, PairLess> now;
	for (size_t i = 0; i < active.size(); ++i)
	{
		for (size_t j = i + 1; j < active.size(); ++j)
		{
			Vector2 pa = active[i].object->GetTransform().position, pb = active[j].object->GetTransform().position;
			float dx = pb.x - pa.x, dy = pb.y - pa.y;
			float reach = active[i].collider->GetRadius() + active[j].collider->GetRadius();
			if (dx * dx + dy * dy < reach * reach) { now.insert(makePair(active[i].object, active[j].object)); }
		}
	}

	vector<ObjectPair> entered, stayed, exited;
	set_difference(now.begin(), now.end(), touchingPairs.begin(), touchingPairs.end(), back_inserter(entered), PairLess());
	set_intersection(now.begin(), now.end(), touchingPairs.begin(), touchingPairs.end(), back_inserter(stayed), PairLess());
	set_difference(touchingPairs.begin(), touchingPairs.end(), now.begin(), now.end(), back_inserter(exited), PairLess());
	touchingPairs = now; 

	auto deliver = [this](const ObjectPair& pair, void (Object::* event)(Object*))
		{
			if (!IsMarked(pair.first)) { (pair.first->*event)(pair.second); }
			if (!IsMarked(pair.second)) { (pair.second->*event)(pair.first); }
		};

	for (const ObjectPair& pair : entered) { deliver(pair, &Object::OnCollisionEnter); }
	for (const ObjectPair& pair : stayed) { deliver(pair, &Object::OnCollisionStay); }
	for (const ObjectPair& pair : exited) { deliver(pair, &Object::OnCollisionExit); }

	const MapArray& wallValues = GetWalls();
	const float T = (float)TILE_SIZE;

	for (const Participant& participant : active)
	{
		Object* object = participant.object;
		if (IsMarked(object) || !participant.collider->IsActive()) { continue; }  

		Vector2 p = object->GetTransform().position;
		float reach = participant.collider->GetRadius() + WallTouchMargin;
		int x0 = (int)floorf((p.x - reach) / T), x1 = (int)floorf((p.x + reach) / T);
		int y0 = (int)floorf((p.y - reach) / T), y1 = (int)floorf((p.y + reach) / T);

		vector<Tile> current;
		for (int ty = y0; ty <= y1; ++ty)
		{
			for (int tx = x0; tx <= x1; ++tx)
			{
				if (tx < 0 || ty < 0 || tx >= MAP_TILES_X || ty >= MAP_TILES_Y) { continue; } 
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

		for (const Tile& w : wallsEntered) { if (!IsMarked(object)) { object->OnWallEnter(w.x, w.y, w.value); } }
		for (const Tile& w : wallsStayed) { if (!IsMarked(object)) { object->OnWallStay(w.x, w.y, w.value); } }
		for (const Tile& w : wallsExited) { if (!IsMarked(object)) { object->OnWallExit(w.x, w.y, w.value); } }
	}

	unordered_set<Object*> activeSet;
	for (const Participant& participant : active) { activeSet.insert(participant.object); }
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

void Level::RemoveDestroyed()
{
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		vector<ObjectPair> goodbyes;
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

	RunCollisions();
	RemoveDestroyed();
}

void Level::Destroy(Object* object)
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

void Level::DestroyAllObjects()
{
	for (ObjectEntry& entry : objects) 
	{ 
		entry.destroyed = true;	
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

