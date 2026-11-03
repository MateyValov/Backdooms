#pragma once

#include <array>

enum Keystate
{
	none,
	pressed,
	held,
	released
};


class Input
{
private:
	inline static std::array<std::pair<bool, bool>, 256> keyStates;

	static bool keyDown(int vk);

public:
	static void InitKeyStates();
	static void updateKeys();
	static Keystate GetKeyState(int key);
};