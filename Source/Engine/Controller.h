#pragma once

#define SC_W 0x11
#define SC_A 0x1E
#define SC_S 0x1F
#define SC_D 0x20


typedef struct
{
	int w, a, s, d;
}ButtonKeys;

typedef struct
{
	int Forward;
	int Right;
}MovementInput;

extern ButtonKeys Keys;

static bool physicalKeyDown(int scancode);
static bool vkDown(int vk);
void updateKeys();
