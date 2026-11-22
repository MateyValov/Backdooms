#pragma once

class Level;

namespace Engine
{
	bool InitEngine(int* argc, char** argv, int screenWidth, int screenHeight);
	void OpenLevel(Level* level);
	void Quit();

	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float value);
};