#pragma once


class Level;

namespace Engine
{
	bool InitEngine(int* argc, char** argv, int screenWidth, int screenHeight);

	Level* GetLevel();

	float GetDeltaTime();
};