#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "../Structs/Transform.h"
#include "../Structs/MapArray.h"

using namespace std;

class Object;

class Map
{
private:
	MapArray walls;
	MapArray floor;
	MapArray roof;

	struct ObjectEntry
	{
		unique_ptr<Object> object;
		bool destroyed = false;        
	};
	vector<ObjectEntry> objects;

	bool IsSolid(int tileX, int tileY) const;
	bool OverlapsWall(const Vector2& centre, float radius) const;

public:
	Map();

	void GenerateMap();
	void ResetMap();

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;
	Vector2 GetCenter() const;

	int GetTile(int x, int y) const;

	Vector2 EvaluatePosition(Vector2 position, float radius);

	template <typename T, typename... Args>
	T* Spawn(Vector2 position, float angle, Args&&... args);
	
	void UpdateObjects();

	void Destroy(Object* object);
	void DestroyAllObjects(bool includePlayer = false);
	
	int ObjectCount() const;      
};

template<typename T, typename ...Args>
inline T* Map::Spawn(Vector2 position, float angle, Args && ...args)
{
	static_assert(is_base_of<Object, T>::value, "Map::Spawn<T>: T must inherit from Object");

	unique_ptr<T> owned = make_unique<T>(std::forward<Args>(args)...);
	T* object = owned.get();
	object->SetTransform(Transform(position, angle));
	objects.push_back(ObjectEntry{ move(owned), false });
	object->Start();
	return object;	
}
