#pragma once

class Map;
class Player;

namespace Engine
{
	bool InitEngine(int* argc, char** argv);

	Player* GetMainPlayer();
	
	Map* GetMap();

	float GetDeltaTime();
};