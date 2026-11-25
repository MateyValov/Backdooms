#pragma once

#include "Architecture/Level.h"

class BackdoomsLevel : public Level
{
public:
	BackdoomsLevel() = default;

	void GenerateInitialObjects() override;
	void GenerateLevel() override;

};