#include "Input.h"

#include <windows.h>

bool Input::keyDown(int vk)
{
	int i = VK_SPACE;
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

void Input::InitKeyStates()
{
	for (int i = 0; i < 256; i++)
	{
		keyStates[i] = std::make_pair(false, false);
	}
}

void Input::updateKeys()
{
	if (GetActiveWindow() == NULL)
	{
		return;
	}

	for (int i = 0; i < 256; i++)
	{
		keyStates[i].second = keyStates[i].first;
		keyStates[i].first = keyDown(i);
	}
}

Keystate Input::GetKeyState(int key)
{
	if (keyStates[key].first && !keyStates[key].second)
	{
		return pressed;
	}
	if (keyStates[key].first && keyStates[key].second)
	{
		return held;
	}
	if (!keyStates[key].first && keyStates[key].second)
	{
		return released;
	}
	return none;
}
