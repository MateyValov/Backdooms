#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "Source/Engine/ImageLoader.h"
#include "Source/Engine/Map.h"
#include "Source/Engine/Controller.h"

#include "Source/Objects/Player.h"

#define PI 3.141593
//#define PI_LARGE 3.1415926535
//#define RAD_90 (PI / 2)
//#define RAD_270 (3 * PI / 2)
//#define DEGREE_RADIAN 0.017453

#define RESOLUTION_WINDOW_X 640 
#define RESOLUTION_WINDOW_Y 480

#define MAX_RAY_STEPS 20

#define BOTTOM_MARGIN 150                                 // free space under the 3D view
#define VIEW_W RESOLUTION_WINDOW_X                         // 3D view is as wide as the window
#define VIEW_H (RESOLUTION_WINDOW_Y - BOTTOM_MARGIN)       // 412; keep this number even

#define FOV_DEGREES 60                                     // real field of view, in degrees
#define COLUMN_W 4                                         // pixels per ray; must divide VIEW_W exactly
#define RAY_COUNT (VIEW_W / COLUMN_W)                      // 256 rays
#define RAY_STEP ((float)FOV_DEGREES / RAY_COUNT)          // degrees between neighbouring rays

#define PROJ_DIST VIEW_H   // how tall walls look: a wall one tile away fills the view height

using namespace std;

unsigned char floorBuf[VIEW_W * VIEW_H * 4];
GLuint floorBufTex = 0;

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

void drawWalls()
{
	int mx, my, mp, dof;
	float rx = 0, ry = 0, xo = 0, yo = 0;

	float ra = FixAng(PLAYER_ANGLE + FOV_DEGREES / 2.0f - RAY_STEP / 2);

	for (int r = 0; r < RAY_COUNT; r++)
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
		//glLineWidth(3); glBegin(GL_LINES); glVertex2i(PLAYER_X, PLAYER_Y); glVertex2i(rx, ry); glEnd();

		// ---- fisheye correction ----
		float ca = FixAng(PLAYER_ANGLE - ra);
		disT = disT * cos(degToRad(ca));

		// ---- textured wall column ----
		float lineH = (TILE_SIZE * PROJ_DIST) / disT;
		float cut = 0;   // fraction of the wall that falls off-screen at the top and bottom
		if (lineH > VIEW_H) { cut = (lineH - VIEW_H) / (2 * lineH); lineH = VIEW_H; }
		float lineO = VIEW_H / 2.0f - lineH / 2;   // horizon is the middle of the view

		int tile = (tileHit > 0 ? tileHit - 1 : 0) % tileCount;   // map value 1 -> first tile in the image
		float tileV = 1.0f / tileCount;
		float eps = 0.5f / texH;                                  // keeps neighbouring tiles from bleeding in
		float v0 = (tile + cut) * tileV + eps;
		float v1 = (tile + 1 - cut) * tileV - eps;

		float x0 = r * COLUMN_W, x1 = x0 + COLUMN_W;

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

		ra = FixAng(ra - RAY_STEP); 
	}
}

static void sampleTile(unsigned char* out, int tile, float u, float v, const unsigned char* fallback)
{
	if (tile > 0 && !tilesetPixels.empty())
	{
		const unsigned char* px = tilesetPixels.data();   // plain pointer: much faster than vector[] in Debug builds
		int t = (tile - 1) % tileCount;
		int tx = (int)(u * texW); if (tx < 0) { tx = 0; } if (tx > texW - 1) { tx = texW - 1; }
		int ty = (int)(v * texW); if (ty < 0) { ty = 0; } if (ty > texW - 1) { ty = texW - 1; }
		const unsigned char* src = &px[((t * texW + ty) * texW + tx) * 4];
		out[0] = src[0]; out[1] = src[1]; out[2] = src[2]; out[3] = 255;
	}
	else
	{
		out[0] = fallback[0]; out[1] = fallback[1]; out[2] = fallback[2]; out[3] = 255;
	}
}

void buildFloorAndCeiling()
{
	static const unsigned char FLOOR_COLOUR[3] = { 40, 40, 40 };
	static const unsigned char CEILING_COLOUR[3] = { 60, 60, 75 };

	// Ray direction for every pixel column, worked out once per frame.
	static float rdx[VIEW_W], rdy[VIEW_W], invCos[VIEW_W];
	for (int x = 0; x < VIEW_W; x++)
	{
		float theta = FOV_DEGREES / 2.0f - (x + 0.5f) * ((float)FOV_DEGREES / VIEW_W);   // degrees off the view direction, positive = left
		float a = degToRad(PLAYER_ANGLE + theta);
		rdx[x] = cos(a);
		rdy[x] = -sin(a);
		invCos[x] = 1.0f / cos(degToRad(theta));
	}

	const int half = VIEW_H / 2;
	for (int y = half; y < VIEW_H; y++)               // bottom half of the view = floor
	{
		float perp = (TILE_SIZE / 2.0f) * PROJ_DIST / (y + 0.5f - half);   // distance straight ahead that this row sees
		int yc = VIEW_H - 1 - y;                                           // mirrored row in the top half = ceiling

		for (int x = 0; x < VIEW_W; x++)
		{
			float t = perp * invCos[x];               // distance along this column's ray
			float wx = PLAYER_X + rdx[x] * t;
			float wy = PLAYER_Y + rdy[x] * t;
			int cellX = (int)floorf(wx / TILE_SIZE);
			int cellY = (int)floorf(wy / TILE_SIZE);

			unsigned char* fp = &floorBuf[(y * VIEW_W + x) * 4];
			unsigned char* cp = &floorBuf[(yc * VIEW_W + x) * 4];

			if (cellX >= 0 && cellX < MAP_TILES_X && cellY >= 0 && cellY < MAP_TILES_Y)
			{
				float u = wx / TILE_SIZE - cellX;
				float v = wy / TILE_SIZE - cellY;
				sampleTile(fp, ground[cellY * MAP_TILES_X + cellX], u, v, FLOOR_COLOUR);
				sampleTile(cp, ceiling[cellY * MAP_TILES_X + cellX], u, v, CEILING_COLOUR);
			}
			else
			{
				sampleTile(fp, 0, 0, 0, FLOOR_COLOUR);
				sampleTile(cp, 0, 0, 0, CEILING_COLOUR);
			}
		}
	}
}

void initFloorBuffer()
{
	glGenTextures(1, &floorBufTex);
	glBindTexture(GL_TEXTURE_2D, floorBufTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, VIEW_W, VIEW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void drawFloorAndCeiling()
{
	buildFloorAndCeiling();

	glBindTexture(GL_TEXTURE_2D, floorBufTex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, VIEW_W, VIEW_H, GL_RGBA, GL_UNSIGNED_BYTE, floorBuf);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(VIEW_W, 0);
	glTexCoord2f(1, 1); glVertex2f(VIEW_W, VIEW_H);
	glTexCoord2f(0, 1); glVertex2f(0, VIEW_H);
	glEnd();
	glDisable(GL_TEXTURE_2D);
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
	//drawMap();
	//drawPlayer();
	drawFloorAndCeiling();
	drawWalls();
	
	glutSwapBuffers();
}

int init()
{
	if (!loadTileset()) { return 1; }
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
	glutKeyboardFunc(ButtonDown);
	glutKeyboardUpFunc(ButtonUp);

	glutMainLoop();
}
