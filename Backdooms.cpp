#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "Source/Engine/ImageLoader.h"
#include "Source/Engine/Map.h"
#include "Source/Engine/Controller.h"

#define PI 3.141593
//#define PI_LARGE 3.1415926535
//#define RAD_90 (PI / 2)
//#define RAD_270 (3 * PI / 2)
//#define DEGREE_RADIAN 0.017453

#define RESOLUTION_WINDOW_X 1024
#define RESOLUTION_WINDOW_Y 512

#define RESOLUTION_GAME_X 320
#define RESOLUTION_GAME_Y 160

#define PLAYER_SPEED 200   
#define TURN_SPEED 150    
#define MAX_RAY_STEPS 5
#define PLAYER_FOV 60

#define GAME_WINDOW_OFFSET (MAP_TILES_X * (TILE_SIZE + 1))

float PLAYER_X;
float PLAYER_Y;
float PLAYER_DELTA_X = 0.f;
float PLAYER_DELTA_Y = 0.f;
float PLAYER_ANGLE = 0.f;

using namespace std;

float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx-ax)*(bx-ax) + (by - ay) * (by - ay)));
}

float degToRad(float a)
{
	return a * PI / 180;
}

float FixAng(float a)
{
	if (a >= 360) { a -= 360; }
	if (a < 0) { a += 360; }
	return a;
}

void drawRays3D()
{
	int mx, my, mp, dof;
	float rx = 0, ry = 0, xo = 0, yo = 0;

	float ra = FixAng(PLAYER_ANGLE + PLAYER_FOV / 2);   // leftmost ray

	for (int r = 0; r < PLAYER_FOV; r++)
	{
		float raRad = degToRad(ra);

		// ---- vertical grid lines ----
		dof = 0;
		float disV = 1000000, vx = PLAYER_X, vy = PLAYER_Y;
		int vTile = 0;
		float Tan = tan(raRad);
		if (cos(raRad) > 0.001)        // looking right
		{
			rx = (((int)PLAYER_X >> 6) << 6) + TILE_SIZE;
			ry = (PLAYER_X - rx) * Tan + PLAYER_Y;
			xo = TILE_SIZE;  yo = -xo * Tan;
		}
		else if (cos(raRad) < -0.001)  // looking left
		{
			rx = (((int)PLAYER_X >> 6) << 6) - 0.0001;
			ry = (PLAYER_X - rx) * Tan + PLAYER_Y;
			xo = -TILE_SIZE; yo = -xo * Tan;
		}
		else { rx = PLAYER_X; ry = PLAYER_Y; dof = MAX_RAY_STEPS; }   // straight up/down

		while (dof < MAX_RAY_STEPS)
		{
			mx = (int)rx >> 6; my = (int)ry >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && walls[mp] > 0)
			{
				vx = rx; vy = ry; vTile = walls[mp]; disV = dist(PLAYER_X, PLAYER_Y, vx, vy, ra); dof = MAX_RAY_STEPS;
			}
			else { rx += xo; ry += yo; dof++; }
		}

		// ---- horizontal grid lines ----
		dof = 0;
		float disH = 1000000, hx = PLAYER_X, hy = PLAYER_Y;
		int hTile = 0;
		Tan = 1.0 / Tan;
		if (sin(raRad) > 0.001)        // looking up
		{
			ry = (((int)PLAYER_Y >> 6) << 6) - 0.0001;
			rx = (PLAYER_Y - ry) * Tan + PLAYER_X;
			yo = -TILE_SIZE; xo = -yo * Tan;
		}
		else if (sin(raRad) < -0.001)  // looking down
		{
			ry = (((int)PLAYER_Y >> 6) << 6) + TILE_SIZE;
			rx = (PLAYER_Y - ry) * Tan + PLAYER_X;
			yo = TILE_SIZE;  xo = -yo * Tan;
		}
		else { rx = PLAYER_X; ry = PLAYER_Y; dof = MAX_RAY_STEPS; }   // straight left/right

		while (dof < MAX_RAY_STEPS)
		{
			mx = (int)rx >> 6; my = (int)ry >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && walls[mp] > 0)
			{
				hx = rx; hy = ry; hTile = walls[mp]; disH = dist(PLAYER_X, PLAYER_Y, hx, hy, ra); dof = MAX_RAY_STEPS;
			}
			else { rx += xo; ry += yo; dof++; }
		}

		// ---- pick the closer hit ----
		float disT, u, shade;
		int tileHit;
		if (disV < disH)
		{
			rx = vx; ry = vy; disT = disV; shade = 0.9f;
			tileHit = vTile;
			u = fmod(vy, TILE_SIZE) / TILE_SIZE;
			if (cos(raRad) < 0) { u = 1 - u; }   // facing left: flip so it isn't mirrored
		}
		else
		{
			rx = hx; ry = hy; disT = disH; shade = 0.7f;
			tileHit = hTile;
			u = fmod(hx, TILE_SIZE) / TILE_SIZE;
			if (sin(raRad) < 0) { u = 1 - u; }   // facing down: flip
		}

		glColor3f(shade, 0, 0);
		glLineWidth(3); glBegin(GL_LINES); glVertex2i(PLAYER_X, PLAYER_Y); glVertex2i(rx, ry); glEnd();

		// ---- fisheye correction ----
		float ca = FixAng(PLAYER_ANGLE - ra);
		disT = disT * cos(degToRad(ca));

		// ---- textured wall column ----
		float lineH = (TILE_SIZE * RESOLUTION_GAME_X) / disT;
		float cut = 0;   // fraction of the wall that falls off-screen at the top and bottom
		if (lineH > RESOLUTION_GAME_X) { cut = (lineH - RESOLUTION_GAME_X) / (2 * lineH); lineH = RESOLUTION_GAME_X; }
		float lineO = RESOLUTION_GAME_Y - lineH / 2;

		int tile = (tileHit > 0 ? tileHit - 1 : 0) % tileCount;   // map value 1 -> first tile in the image
		float tileV = 1.0f / tileCount;
		float eps = 0.5f / texH;                                  // keeps neighbouring tiles from bleeding in
		float v0 = (tile + cut) * tileV + eps;
		float v1 = (tile + 1 - cut) * tileV - eps;

		float x0 = r * 8 + GAME_WINDOW_OFFSET, x1 = x0 + 8;
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tilesetTex);
		glColor3f(shade, shade, shade);   // darkens side walls; the texture gets multiplied by this
		glBegin(GL_QUADS);
		glTexCoord2f(u, v0); glVertex2f(x0, lineO);
		glTexCoord2f(u, v0); glVertex2f(x1, lineO);
		glTexCoord2f(u, v1); glVertex2f(x1, lineO + lineH);
		glTexCoord2f(u, v1); glVertex2f(x0, lineO + lineH);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		ra = FixAng(ra - 1);   // next ray, one degree to the right
	}
}

void drawPlayer()
{
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(PLAYER_X, PLAYER_Y);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(PLAYER_X, PLAYER_Y);
	glVertex2i(PLAYER_X + PLAYER_DELTA_X * 5, PLAYER_Y + PLAYER_DELTA_Y * 5);
	glEnd();
}

void drawMap()
{
	int x, y, xo, yo;
	for (y = 0; y < MAP_TILES_Y; y++)
	{
		for (x = 0; x < MAP_TILES_X; x++)
		{
			if (walls[y * MAP_TILES_X + x] > 0) { glColor3f(1, 1, 1); }
			else { glColor3f(0, 0, 0); }
			xo = x * TILE_SIZE; yo = y * TILE_SIZE;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + TILE_SIZE - 1);
			glVertex2i(xo + TILE_SIZE - 1, yo + 1);
			glEnd();
		}
	}
}

float lastTime = 0, deltaTime = 0;

void display()
{
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
	drawMap();
	drawPlayer();
	drawRays3D();
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y, 0);
	PLAYER_X = 200.f;
	PLAYER_Y = 200.f;
	PLAYER_DELTA_X = cos(degToRad(PLAYER_ANGLE));
	PLAYER_DELTA_Y = -sin(degToRad(PLAYER_ANGLE));
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

	if (!loadTileset()) { return 1; }

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(ButtonDown);
	glutKeyboardUpFunc(ButtonUp);

	glutMainLoop();
}
