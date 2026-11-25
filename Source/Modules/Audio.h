#pragma once

#include <string>

class Audio
{
public:
	static bool Init();       
	static void Shutdown();

	static void PlaySoundEffect(const std::string& filename);

	static int PlayLoop(const std::string& filename);
	static void StopLoop(int handle);
	static void StopAllLoops();
};