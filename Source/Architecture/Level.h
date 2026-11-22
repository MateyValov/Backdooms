#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <functional>
#include <set>

#include "Structs/Transform.h"
#include "Structs/Tile.h"

using namespace std;

class Object;
class Collider;
class Viewport;

struct ObjectEntry
{
	unique_ptr<Object> object;
	bool destroyed = false;
};     

class Level
{
protected:
	MapArray walls = MapArray();
	MapArray floor = MapArray();
	MapArray roof = MapArray();

	vector<ObjectEntry> objects;

public:
	Level() = default;
	virtual ~Level() = default;

	virtual void GenerateLevel() {}
	virtual void GenerateInitialObjects() {}

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;

	Vector2 GetSpawnpoint() const;

	int GetTile(int x, int y) const;
	vector<Object*> GetObjects() const;

	void SetWallTile(int x, int y, int value);
	void SetFloorTile(int x, int y, int value);
	void SetRoofTile(int x, int y, int value);

	bool IsSolid(int tileX, int tileY) const;
	bool OverlapsWall(const Vector2& centre, float radius) const;

	template <typename T>
	vector<T*> GetObjectsOfClass();

	Vector2 EvaluatePosition(Object* obj, Vector2 position);

	template <typename T, typename... Args>
	T* Spawn(Vector2 position, float angle, Args&&... args);
	
	void UpdateObjects();

	bool IsMarked(const Object* object) const;
	void RemoveDestroyed();

	void Destroy(Object* object);
	void DestroyAllObjects();
	
	int ObjectCount() const;    
};

template<typename T>
inline vector<T*> Level::GetObjectsOfClass()
{
	static_assert(is_base_of<Object, T>::value, "Level::Spawn<T>: T must inherit from Object");

	vector<T*> existing;
	existing.reserve(objects.size());
	for (const ObjectEntry& entry : objects)
	{
		if (entry.destroyed)
		{
			continue;
		}
		T* obj = dynamic_cast<T*>(entry.object.get());
		if (obj == nullptr)
		{
			continue;
		}
		existing.push_back(obj);
	}
	return existing;
}

template<typename T, typename ...Args>
inline T* Level::Spawn(Vector2 position, float angle, Args && ...args)
{
	static_assert(is_base_of<Object, T>::value, "Level::Spawn<T>: T must inherit from Object");

	unique_ptr<T> owned = make_unique<T>(std::forward<Args>(args)...);
	T* object = owned.get();
	objects.push_back(ObjectEntry{ move(owned), false });

	object->level = this;
	object->SetTransform(Transform(position, angle));
	object->Start();

	return object;	
}
