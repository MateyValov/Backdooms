#pragma once

#include "Architecture/Level.h"

class Pickup;
class Monster;

class BackdoomsLevel : public Level
{
public:
	BackdoomsLevel() = default;

	void GenerateInitialObjects() override;
	void GenerateLevel() override;
};