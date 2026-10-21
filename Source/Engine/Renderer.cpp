#include "Renderer.h"

#include <iostream>
#include <GL/glut.h>
#include <math.h>

#include "ImageLoader.h"
#include "Controller.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"


bool Renderer::loadTextures()
{
	imageLoader = new ImageLoader;

	if (!imageLoader->loadTileset()) { return 1; }

	if (!imageLoader->loadImage("Textures/UI.bmp", gui)) { return 1; }
	if (!imageLoader->loadImage("Textures/BarOverlay.png", overlay)) { return 1; }
	if (!imageLoader->loadImage("Textures/Bar.bmp", healthBar)) { return 1; }
	if (!imageLoader->loadImage("Textures/Bar.bmp", ammoBar)) { return 1; }
	if (!imageLoader->loadImage("Textures/Player.png", PlayerIcon)) { return 1; }

	return 0;
}

void Renderer::drawWalls(MapArray walls, Transform viewpoint)
{
	int mx, my, mp, dof;
	float rx = 0, ry = 0, xo = 0, yo = 0;

	float ra = FixAng(viewpoint.angle + FOV_DEGREES / 2.0f - RAY_STEP / 2);

	for (int r = 0; r < RAY_COUNT; r++)
	{
		float raRad = degToRad(ra);

		// ---- vertical grid lines ----
		dof = 0;
		float disV = 1000000, vx = viewpoint.position.x, vy = viewpoint.position.y;
		int vTile = 0;
		float Tan = tan(raRad);
		if (cos(raRad) > 0.001)        // looking right
		{
			rx = (((int)viewpoint.position.x >> 6) << 6) + TILE_SIZE;
			ry = (viewpoint.position.x - rx) * Tan + viewpoint.position.y;
			xo = TILE_SIZE;  yo = -xo * Tan;
		}
		else if (cos(raRad) < -0.001)  // looking left
		{
			rx = (((int)viewpoint.position.x >> 6) << 6) - 0.0001;
			ry = (viewpoint.position.x - rx) * Tan + viewpoint.position.y;
			xo = -TILE_SIZE; yo = -xo * Tan;
		}
		else { rx = viewpoint.position.x; ry = viewpoint.position.y; dof = MAX_RAY_STEPS; }   // straight up/down

		while (dof < MAX_RAY_STEPS)
		{
			mx = (int)rx >> 6; my = (int)ry >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && walls[mp] > 0)
			{
				vx = rx; vy = ry; vTile = walls[mp]; disV = dist(viewpoint.position.x, viewpoint.position.y, vx, vy, ra); dof = MAX_RAY_STEPS;
			}
			else { rx += xo; ry += yo; dof++; }
		}

		// ---- horizontal grid lines ----
		dof = 0;
		float disH = 1000000, hx = viewpoint.position.x, hy = viewpoint.position.y;
		int hTile = 0;
		Tan = 1.0 / Tan;
		if (sin(raRad) > 0.001)        // looking up
		{
			ry = (((int)viewpoint.position.y >> 6) << 6) - 0.0001;
			rx = (viewpoint.position.y - ry) * Tan + viewpoint.position.x;
			yo = -TILE_SIZE; xo = -yo * Tan;
		}
		else if (sin(raRad) < -0.001)  // looking down
		{
			ry = (((int)viewpoint.position.y >> 6) << 6) + TILE_SIZE;
			rx = (viewpoint.position.y - ry) * Tan + viewpoint.position.x;
			yo = TILE_SIZE;  xo = -yo * Tan;
		}
		else { rx = viewpoint.position.x; ry = viewpoint.position.y; dof = MAX_RAY_STEPS; }   // straight left/right

		while (dof < MAX_RAY_STEPS)
		{
			mx = (int)rx >> 6; my = (int)ry >> 6; mp = my * MAP_TILES_X + mx;
			if (mx >= 0 && mx < MAP_TILES_X && my >= 0 && my < MAP_TILES_Y && walls[mp] > 0)
			{
				hx = rx; hy = ry; hTile = walls[mp]; disH = dist(viewpoint.position.x, viewpoint.position.y, hx, hy, ra); dof = MAX_RAY_STEPS;
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
		float ca = FixAng(viewpoint.angle - ra);
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

void Renderer::sampleTile(unsigned char* out, int tile, float u, float v, const unsigned char* fallback)
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

void Renderer::initFloorBuffer()
{
	glGenTextures(1, &floorBufTex);
	glBindTexture(GL_TEXTURE_2D, floorBufTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, VIEW_W, VIEW_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void Renderer::drawFloorAndRoof(MapArray floor, MapArray roof, Transform viewpoint)
{
	static const unsigned char FLOOR_COLOUR[3] = { 40, 40, 40 };
	static const unsigned char ROOF_COLOUR[3] = { 60, 60, 75 };

	// Ray direction for every pixel column, worked out once per frame.
	static float rdx[VIEW_W], rdy[VIEW_W], invCos[VIEW_W];
	for (int x = 0; x < VIEW_W; x++)
	{
		float theta = FOV_DEGREES / 2.0f - (x + 0.5f) * ((float)FOV_DEGREES / VIEW_W);   // degrees off the view direction, positive = left
		float a = degToRad(viewpoint.angle + theta);
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
			float wx = viewpoint.position.x + rdx[x] * t;
			float wy = viewpoint.position.y + rdy[x] * t;
			int cellX = (int)floorf(wx / TILE_SIZE);
			int cellY = (int)floorf(wy / TILE_SIZE);

			unsigned char* fp = &floorBuf[(y * VIEW_W + x) * 4];
			unsigned char* cp = &floorBuf[(yc * VIEW_W + x) * 4];

			if (cellX >= 0 && cellX < MAP_TILES_X && cellY >= 0 && cellY < MAP_TILES_Y)
			{
				float u = wx / TILE_SIZE - cellX;
				float v = wy / TILE_SIZE - cellY;
				sampleTile(fp, floor[cellY * MAP_TILES_X + cellX], u, v, FLOOR_COLOUR);
				sampleTile(cp, roof[cellY * MAP_TILES_X + cellX], u, v, ROOF_COLOUR);
			}
			else
			{
				sampleTile(fp, 0, 0, 0, FLOOR_COLOUR);
				sampleTile(cp, 0, 0, 0, ROOF_COLOUR);
			}
		}
	}

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

void Renderer::DrawMap(Map* map, Transform viewpoint)
{
	drawFloorAndRoof(map->GetFloor(), map->GetRoof(), viewpoint);
	drawWalls(map->GetWalls(), viewpoint);
}

void Renderer::drawUI()
{
	imageLoader->drawImage(gui, Vector2(0, VIEW_H));

	imageLoader->drawImage(healthBar, Vector2(PROGRESS_BAR_OFFSET_V, VIEW_H + HEALTH_BAR_OFFSET_H), 0, 0, Color::Red());
	imageLoader->drawImage(ammoBar, Vector2(PROGRESS_BAR_OFFSET_V, VIEW_H + AMMO_BAR_OFFSET_H), 0, 0, Color::Green());

	imageLoader->drawImage(overlay, Vector2(0, VIEW_H));
	imageLoader->drawImage(PlayerIcon, Vector2(PLAYER_ICON_OFFSET_H, VIEW_H + PLAYER_ICON_OFFSET_V));
}
