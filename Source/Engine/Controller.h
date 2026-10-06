#pragma once

typedef struct
{
	int w, a, s, d;
}ButtonKeys;

extern ButtonKeys Keys;

void ButtonDown(unsigned char key, int x, int y);
void ButtonUp(unsigned char key, int x, int y);
