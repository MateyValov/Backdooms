#pragma once

#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <functional>
#include <set>

#include "Structs/Transform.h"
#include "Structs/Tile.h"

#include "Architecture/Camera.h"
#include "Architecture/Viewport.h"

using namespace std;

class Object;
class Collider;
class GameState;

struct ObjectEntry
{
	unique_ptr<Object> object;
	bool destroyed = false;
};

class Level
{
protected:
	Viewport* globalViewport = nullptr;
	GameState* gameState = nullptr;

	MapArray walls = MapArray();
	MapArray floor = MapArray();
	MapArray roof = MapArray();

	vector<ObjectEntry> objects;
	vector<Camera*> cameras;
	vector<Viewport*> viewports;

	void Register(Object*) {}
	void Register(Camera* camera) { cameras.push_back(camera); }
	void Register(Viewport* viewport) { viewports.push_back(viewport); }

	void Unregister(Object* object);

public:
	Level() = default;
	virtual ~Level() = default;

	virtual void GenerateLevel() {}
	virtual void GenerateInitialObjects() {}

	Viewport* GetGlobalViewport();
	GameState* GetGameState();

	MapArray GetWalls() const;
	MapArray GetFloor() const;
	MapArray GetRoof() const;

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
	
	int ObjectCount() const;  
	vector<Camera*> GetCameras() const;
	vector<Viewport*> GetViewports() const;
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
	Register(object);
	object->Start();

	return object;	
}
