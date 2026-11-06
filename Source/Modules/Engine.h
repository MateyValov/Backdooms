#pragma once

#include "../Structs/Transform.h"
#include "../Structs/Screenspace.h"

class Level;
class Player;

namespace Engine
{
	bool InitEngine(int* argc, char** argv, int screenWidth, int screenHeight);

	Level* GetLevel();

	float GetDeltaTime();

	Vector2 GetWindowSize();

	Screenspace GetFullscreenBounds();
};