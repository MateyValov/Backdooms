#pragma once

#include <windows.h>

struct ButtonKeys
{
	int w, a, s, d, q;
	int up, left, down, right, space;
	int enter, esc;
};

struct PlayerInput
{
	int forward, left, right, back, interact;
};

extern ButtonKeys Keys;

static bool keyDown(UINT vk);
void updateKeys();

