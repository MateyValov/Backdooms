#include "Controller.h"

ButtonKeys Keys;

void ButtonDown(unsigned char key, int x, int y)
{
	if (key == 'w') { Keys.w = 1; }
	if (key == 'a') { Keys.a = 1; }
	if (key == 's') { Keys.s = 1; }
	if (key == 'd') { Keys.d = 1; }
}

void ButtonUp(unsigned char key, int x, int y)
{
	if (key == 'w') { Keys.w = 0; }
	if (key == 'a') { Keys.a = 0; }
	if (key == 's') { Keys.s = 0; }
	if (key == 'd') { Keys.d = 0; }
}