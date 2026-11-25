#include "Audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <memory>
#include <unordered_map>

namespace
{
	const std::string AUDIO_FOLDER = "Assets/Audio/";

	ma_engine engine;
	bool initialized = false;

	std::unordered_map<int, std::unique_ptr<ma_sound>> loops;
	int nextHandle = 1;
}

bool Audio::Init()
{
	if (initialized)
	{
		return true;
	}
	initialized = ma_engine_init(nullptr, &engine) == MA_SUCCESS;
	return initialized;
}

void Audio::Shutdown()
{
	if (initialized == false)
	{
		return;
	}
	StopAllLoops();
	ma_engine_uninit(&engine);
	initialized = false;
}

void Audio::PlaySoundEffect(const std::string& filename)
{
	if (initialized == false)
	{
		return;
	}
	std::string path = AUDIO_FOLDER + filename;
	ma_engine_play_sound(&engine, path.c_str(), nullptr);
}

int Audio::PlayLoop(const std::string& filename)
{
	if (initialized == false)
	{
		return -1;
	}

	std::string path = AUDIO_FOLDER + filename;
	std::unique_ptr<ma_sound> sound = std::make_unique<ma_sound>();

	if (ma_sound_init_from_file(&engine, path.c_str(), 0, nullptr, nullptr, sound.get()) != MA_SUCCESS)
	{
		return -1;
	}

	ma_sound_set_looping(sound.get(), MA_TRUE);
	ma_sound_start(sound.get());

	int handle = nextHandle++;
	loops[handle] = std::move(sound);
	return handle;
}

void Audio::StopLoop(int handle)
{
	auto it = loops.find(handle);
	if (it == loops.end())
	{
		return;
	}
	ma_sound_uninit(it->second.get());
	loops.erase(it);
}

void Audio::StopAllLoops()
{
	for (auto& pair : loops)
	{
		ma_sound_uninit(pair.second.get());
	}
	loops.clear();
}