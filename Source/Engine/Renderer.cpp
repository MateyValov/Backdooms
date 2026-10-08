#include "Renderer.h"

#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "ImageLoader.h"
#include "Controller.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"

unsigned char floorBuf[VIEW_W * VIEW_H * 4];
GLuint floorBufTex = 0;

Image gui;
Image overlay;
Image healthBar;
Image ammoBar;
Image PlayerIcon;

float dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
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