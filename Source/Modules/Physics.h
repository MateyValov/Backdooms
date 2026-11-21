#pragma once

#include "Structs/Transform.h"
#include "Structs/Tile.h"

class Object;
class Level;

struct RaycastHit
{
	enum class Type { None, Wall, Object };

	Type type = Type::None;
	float distance = 0;
	Vector2 point;
	Object* object = nullptr;
	Tile tile;
};

namespace Physics
{
	void UpdatePhysics(Level* level);

	void ReleaseDestroyed(Level* level);

	RaycastHit Raycast(Level* level, Vector2 from, Vector2 to, const Object* ignore = nullptr);
}
