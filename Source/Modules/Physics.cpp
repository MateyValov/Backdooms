#include "Physics.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <functional>
#include <iterator>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Architecture/Level.h"
#include "Architecture/Object.h"
#include "Architecture/Components/Collider.h"

using namespace std;

namespace
{
	using ObjectPair = pair<Object*, Object*>;

	struct PairLess
	{
		bool operator()(const ObjectPair& l, const ObjectPair& r) const
		{
			less<Object*> lt;
			if (lt(l.first, r.first)) { return true; }
			if (lt(r.first, l.first)) { return false; }
			return lt(l.second, r.second);
		}
	};

	set<ObjectPair, PairLess> touchingPairs;
	unordered_map<Object*, vector<Tile>> touchingWalls;

	const float WallTouchMargin = 1.f;

	void RunCollisions(Level* level);
}

void Physics::UpdatePhysics(Level* level)
{
	RunCollisions(level);
}

void Physics::ReleaseDestroyed(Level* level)
{
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		vector<ObjectPair> goodbyes;
		for (auto it = touchingPairs.begin(); it != touchingPairs.end(); )
		{
			bool firstGone = level->IsMarked(it->first), secondGone = level->IsMarked(it->second);
			if (!firstGone && !secondGone) { ++it; continue; }
			if (!firstGone) { goodbyes.push_back(ObjectPair(it->first, it->second)); }
			else if (!secondGone) { goodbyes.push_back(ObjectPair(it->second, it->first)); }
			it = touchingPairs.erase(it);
		}
		for (const ObjectPair& goodbye : goodbyes)
		{
			if (!level->IsMarked(goodbye.first))
			{
				goodbye.first->OnCollisionExit(goodbye.second);
			}
			repeat = true;
		}
	}

	for (auto it = touchingWalls.begin(); it != touchingWalls.end(); )
	{
		if (level->IsMarked(it->first)) { it = touchingWalls.erase(it); }
		else { ++it; }
	}
}

RaycastHit Physics::Raycast(Level* level, Vector2 from, Vector2 to, const Object* ignore)
{
	RaycastHit best;
	const float T = (float)TILE_SIZE;
	float dx = to.x - from.x, dy = to.y - from.y;
	float length = sqrtf(dx * dx + dy * dy);
	Vector2 dir = length > 0 ? Vector2(dx / length, dy / length) : Vector2(0, 0);

	auto hitWall = [&](int tx, int ty, float t)
		{
			best.type = RaycastHit::Type::Wall;
			best.distance = t;
			best.point = Vector2(from.x + dir.x * t, from.y + dir.y * t);
			best.tile.x = tx; best.tile.y = ty;
			best.tile.value = (tx >= 0 && ty >= 0 && tx < MAP_TILES_X && ty < MAP_TILES_Y) ? level->GetWalls()[ty * MAP_TILES_X + tx] : 0;
		};

	int tx = (int)floorf(from.x / T), ty = (int)floorf(from.y / T);
	if (level->IsSolid(tx, ty))
	{
		hitWall(tx, ty, 0);
	}
	else if (length > 0)
	{
		int stepX = dir.x < 0 ? -1 : 1, stepY = dir.y < 0 ? -1 : 1;
		float deltaX = dir.x != 0 ? fabsf(T / dir.x) : FLT_MAX, deltaY = dir.y != 0 ? fabsf(T / dir.y) : FLT_MAX;
		float sideX = dir.x == 0 ? FLT_MAX : (dir.x < 0 ? (from.x - tx * T) / -dir.x : ((tx + 1) * T - from.x) / dir.x);
		float sideY = dir.y == 0 ? FLT_MAX : (dir.y < 0 ? (from.y - ty * T) / -dir.y : ((ty + 1) * T - from.y) / dir.y);
		while (true)
		{
			float t;
			if (sideX < sideY) { t = sideX; sideX += deltaX; tx += stepX; }
			else { t = sideY; sideY += deltaY; ty += stepY; }
			if (t > length) { break; }
			if (level->IsSolid(tx, ty)) { hitWall(tx, ty, t); break; }
		}
	}

	for (Object* object : level->GetObjects())
	{
		if (object == nullptr || object == ignore || object->IsActive() == false)
		{
			continue;
		}
		Collider* collider = object->GetComponent<Collider>();
		if (collider == nullptr || collider->IsActive() == false)
		{
			continue;
		}

		Vector2 c = object->GetTransform().position;
		float fx = from.x - c.x, fy = from.y - c.y, r = collider->GetRadius();
		float cc = fx * fx + fy * fy - r * r;
		float t;
		if (cc <= 0) { t = 0; }
		else
		{
			float b = fx * dir.x + fy * dir.y;
			float disc = b * b - cc;
			if (disc < 0) { continue; }
			t = -b - sqrtf(disc);
			if (t < 0 || t > length) { continue; }
		}
		if (best.type == RaycastHit::Type::None || t < best.distance)
		{
			best.type = RaycastHit::Type::Object;
			best.distance = t;
			best.point = Vector2(from.x + dir.x * t, from.y + dir.y * t);
			best.object = object;
		}
	}
	return best;
}

namespace
{
	void RunCollisions(Level* level)
	{
		struct Participant { Object* object; Collider* collider; };
		vector<Participant> active;
		for (Object* object : level->GetObjects())
		{
			Collider* collider = object->GetComponent<Collider>();
			if (collider == nullptr || !collider->IsActive())
			{
				continue;
			}
			active.push_back(Participant{ object, collider });
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

		auto deliver = [level](const ObjectPair& pair, void (Object::* event)(Object*))
			{
				if (!level->IsMarked(pair.first)) { (pair.first->*event)(pair.second); }
				if (!level->IsMarked(pair.second)) { (pair.second->*event)(pair.first); }
			};

		for (const ObjectPair& pair : entered) { deliver(pair, &Object::OnCollisionEnter); }
		for (const ObjectPair& pair : stayed) { deliver(pair, &Object::OnCollisionStay); }
		for (const ObjectPair& pair : exited) { deliver(pair, &Object::OnCollisionExit); }

		const MapArray wallValues = level->GetWalls();
		const float T = (float)TILE_SIZE;

		for (const Participant& participant : active)
		{
			Object* object = participant.object;
			if (level->IsMarked(object) || !participant.collider->IsActive()) { continue; }

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
					if (level->IsSolid(tx, ty)) { current.push_back(Tile{ tx, ty, wallValues[ty * MAP_TILES_X + tx] }); }
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

			for (const Tile& w : wallsEntered) { if (!level->IsMarked(object)) { object->OnWallEnter(w.x, w.y, w.value); } }
			for (const Tile& w : wallsStayed) { if (!level->IsMarked(object)) { object->OnWallStay(w.x, w.y, w.value); } }
			for (const Tile& w : wallsExited) { if (!level->IsMarked(object)) { object->OnWallExit(w.x, w.y, w.value); } }
		}

		unordered_set<Object*> activeSet;
		for (const Participant& participant : active) { activeSet.insert(participant.object); }
		vector<pair<Object*, vector<Tile>>> letGo;
		for (auto it = touchingWalls.begin(); it != touchingWalls.end(); )
		{
			if (activeSet.count(it->first) || level->IsMarked(it->first)) { ++it; continue; }
			letGo.push_back({ it->first, std::move(it->second) });
			it = touchingWalls.erase(it);
		}
		for (auto& entry : letGo)
		{
			for (const Tile& w : entry.second)
			{
				if (!level->IsMarked(entry.first)) { entry.first->OnWallExit(w.x, w.y, w.value); }
			}
		}
	}
}