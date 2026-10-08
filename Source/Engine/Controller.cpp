#include "Controller.h"
#include <windows.h>

ButtonKeys Keys;
MovementInput Input;

static bool physicalKeyDown(int scancode)
{
	UINT vk = MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK);   // which virtual key sits at that position right now
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

void updateKeys()
{
	if (GetActiveWindow() == NULL)
	{
		return;
	}

	Keys.w = physicalKeyDown(SC_W);
	Keys.a = physicalKeyDown(SC_A);
	Keys.s = physicalKeyDown(SC_S);
	Keys.d = physicalKeyDown(SC_D);
}