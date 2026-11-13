#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <functional>
#include <set>
#include <unordered_map>

#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"

using namespace std;

class Object;
class Collider;

using ObjectPair = std::pair<Object*, Object*>;

struct ObjectEntry
{
	unique_ptr<Object> object;
	bool destroyed = false;
};

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

struct RaycastHit
{
	enum class Type { None, Wall, Object };

	Type type = Type::None;
	float distance = 0;                      
	Vector2 point;                             
	Object* object = nullptr;                
	Tile tile;
};

class Level
{
private:
	MapArray walls;
	MapArray floor;
	MapArray roof;

	vector<ObjectEntry> objects;

	bool IsSolid(int tileX, int tileY) const;
	bool OverlapsWall(const Vector2& centre, float radius) const;

	set<ObjectPair, PairLess> touchingPairs;
	unordered_map<Object*, vector<Tile>> touchingWalls;

	bool IsMarked(const Object* object) const;
	void RunCollisions();
	void RemoveDestroyed();

public:
	Level();

	void GenerateLevel();
	void ResetLevel();

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;

	Vector2 GetSpawnpoint() const;

	int GetTile(int x, int y) const;
	vector<Object*> GetObjects() const;

	Vector2 EvaluatePosition(Object* obj, Vector2 position);

	template <typename T, typename... Args>
	T* Spawn(Vector2 position, float angle, Args&&... args);
	
	void UpdateObjects();

	void Destroy(Object* object);
	void DestroyAllObjects();

	RaycastHit Raycast(Vector2 from, Vector2 to, const Object* ignore = nullptr) const;
	
	int ObjectCount() const;    


};

template<typename T, typename ...Args>
inline T* Level::Spawn(Vector2 position, float angle, Args && ...args)
{
	static_assert(is_base_of<Object, T>::value, "Map::Spawn<T>: T must inherit from Object");

	unique_ptr<T> owned = make_unique<T>(std::forward<Args>(args)...);
	T* object = owned.get();
	objects.push_back(ObjectEntry{ move(owned), false });

	object->SetTransform(Transform(position, angle));
	object->Start();

	return object;	
}
