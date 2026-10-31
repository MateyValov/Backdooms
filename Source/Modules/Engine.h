#pragma once

#include "../Structs/Transform.h"
#include "../Structs/Screenspace.h"

class Map;
class Player;


namespace Engine
{
	bool InitEngine(int* argc, char** argv, int screenWidth, int screenHeight);

	Player* GetMainPlayer();
	Map* GetMap();

	float GetDeltaTime();
	Vector2 GetWindowSize();
	Screenspace GetFullscreenBounds();
};