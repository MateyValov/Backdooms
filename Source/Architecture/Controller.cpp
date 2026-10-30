#include "Controller.h"

ButtonKeys Keys;

static bool keyDown(UINT vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

void updateKeys()
{
	if (GetActiveWindow() == NULL)
	{
		return;
	}

	Keys.w = keyDown('W');
	Keys.a = keyDown('A');
	Keys.s = keyDown('S');
	Keys.d = keyDown('D');
	Keys.q = keyDown('Q');

	Keys.up = keyDown(VK_UP);
	Keys.left = keyDown(VK_LEFT);
	Keys.down = keyDown(VK_DOWN);
	Keys.right = keyDown(VK_RIGHT);
	Keys.space = keyDown(VK_SPACE);

	Keys.enter = keyDown(VK_RETURN);
	Keys.esc = keyDown(VK_ESCAPE);
}