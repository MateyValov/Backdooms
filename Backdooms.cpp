#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "Source/Engine/ImageLoader.h"
#include "Source/Engine/Controller.h"
#include "Source/Engine/Renderer.h"

#include "Source/Objects/Player.h"
#include "Source/Objects/Map.h"

using namespace std;

float lastTime = 0, deltaTime = 0;

void display()
{
	updateKeys();

	float now = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (now - lastTime) / 1000.f;
	lastTime = now;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	if (Keys.a == 1)
	{
		PLAYER_ANGLE += TURN_SPEED * deltaTime;
		PLAYER_ANGLE = FixAng(PLAYER_ANGLE);
		PLAYER_DELTA_X = cos(degToRad(PLAYER_ANGLE));
		PLAYER_DELTA_Y =- sin(degToRad(PLAYER_ANGLE));
	}
	if (Keys.d == 1)
	{
		PLAYER_ANGLE -= TURN_SPEED * deltaTime;
		PLAYER_ANGLE = FixAng(PLAYER_ANGLE);
		PLAYER_DELTA_X = cos(degToRad(PLAYER_ANGLE));
		PLAYER_DELTA_Y = -sin(degToRad(PLAYER_ANGLE));
	}

	int xo = 0; if (PLAYER_DELTA_X < 0) { xo = -20; } else { xo = 20; }
	int yo = 0; if (PLAYER_DELTA_Y < 0) { yo = -20; } else { yo = 20; }
	int ipx = PLAYER_X / 64.f, ipx_add_xo = (PLAYER_X + xo) / 64.f, ipx_sub_xo = (PLAYER_X - xo) / 64.f;
	int ipy = PLAYER_Y / 64.f, ipy_add_yo = (PLAYER_Y + yo) / 64.f, ipy_sub_yo = (PLAYER_Y - yo) / 64.f;

	if (Keys.w == 1) 
	{ 
		if (walls[ipy * MAP_TILES_X + ipx_add_xo] == 0) { PLAYER_X += PLAYER_DELTA_X * PLAYER_SPEED * deltaTime; }
		if (walls[ipy_add_yo * MAP_TILES_X + ipx] == 0) { PLAYER_Y += PLAYER_DELTA_Y * PLAYER_SPEED * deltaTime; }
	}
	if (Keys.s == 1) 
	{ 
		if (walls[ipy * MAP_TILES_X + ipx_sub_xo] == 0) { PLAYER_X -= PLAYER_DELTA_X * PLAYER_SPEED * deltaTime; }
		if (walls[ipy_sub_yo * MAP_TILES_X + ipx] == 0) { PLAYER_Y -= PLAYER_DELTA_Y * PLAYER_SPEED * deltaTime; }
	}

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawFloorAndCeiling();
	drawWalls();

	drawImage(gui, 0, VIEW_H, VIEW_W, BOTTOM_MARGIN);
	drawImage(healthBar, PROGRESS_BAR_OFFSET_V, VIEW_H + HEALTH_BAR_OFFSET_H, healthBar.width, healthBar.height);
	drawImage(ammoBar, PROGRESS_BAR_OFFSET_V, VIEW_H + AMMO_BAR_OFFSET_H, ammoBar.width, ammoBar.height);
	drawImage(overlay, 0, VIEW_H, VIEW_W, BOTTOM_MARGIN);
	drawImage(PlayerIcon, PLAYER_ICON_OFFSET_H, VIEW_H + PLAYER_ICON_OFFSET_V, PlayerIcon.width, PlayerIcon.height);
	
	glutSwapBuffers();
}

int init()
{
	if (!loadTileset()) { return 1; }

	if (!loadImage("Textures/UI.bmp", gui)) { return 1; }
	if (!loadImage("Textures/BarOverlay.png", overlay)) { return 1; }
	if (!loadImage("Textures/Bar.bmp", healthBar)) { return 1; }
	if (!loadImage("Textures/Bar.bmp", ammoBar)) { return 1; }
	if (!loadImage("Textures/Player.png", PlayerIcon)) { return 1; }

	GenerateMap();
	initFloorBuffer();

	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y, 0);
	PLAYER_X = (MAP_TILES_X / 2.f) * TILE_SIZE;
	PLAYER_Y = (MAP_TILES_Y / 2.f) * TILE_SIZE;
	PLAYER_DELTA_X = cos(degToRad(PLAYER_ANGLE));
	PLAYER_DELTA_Y = -sin(degToRad(PLAYER_ANGLE));

	return 0;
}

void resize(int w, int h)
{
	glutReshapeWindow(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
	glutCreateWindow("Backdooms");

	if (init() != 0) { return 1; };
	glutDisplayFunc(display);
	glutReshapeFunc(resize);

	glutMainLoop();
}
