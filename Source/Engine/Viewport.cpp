#include "Viewport.h"

#include <algorithm>
#include <cmath>

#include "../Objects/Map.h"  
#include "ImageLoader.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif


namespace
{
	const float PI_F = 3.14159265f;
	const float SHADE_EAST_WEST_FACE = 0.9f;     // walls seen from the east or west
	const float SHADE_NORTH_SOUTH_FACE = 0.7f;   // walls seen from the north or south

	const unsigned char FLOOR_COLOUR[3] = { 40, 40, 40 };       // where the ground map says 0, or off the map
	const unsigned char CEILING_COLOUR[3] = { 60, 60, 75 };
}

Viewport::Viewport()
{
	origin = Vector2();
	width = RESOLUTION_WINDOW_X;
	height = RESOLUTION_WINDOW_Y;
	Setup();
}

Viewport::Viewport(Vector2 origin, int width, int height)
{
	this->origin = origin;
	this->width = width;
	this->height = height;
	Setup();
}

void Viewport::Setup()
{
	columnCount = (width + COLUMN_W - 1) / COLUMN_W;
	projDist = (float)height;

	floorBuf.assign((size_t)width * height * 4, 0);

	// Angle away from the view direction at horizontal position xf (in pixels, 0 = left edge of the viewport).
	auto angleAt = [&](float xf, float& c, float& s)
		{
			float theta = (FOV_DEGREES / 2.0f - xf * ((float)FOV_DEGREES / width)) * PI_F / 180.0f;
			c = cosf(theta);
			s = sinf(theta);
		};

	pxCos.resize(width); pxSin.resize(width); pxInvCos.resize(width);
	pxDirX.resize(width); pxDirY.resize(width);
	for (int x = 0; x < width; ++x)
	{
		angleAt(x + 0.5f, pxCos[x], pxSin[x]);     // centre of pixel x
		pxInvCos[x] = 1.0f / pxCos[x];
	}

	colCos.resize(columnCount); colSin.resize(columnCount);
	for (int c = 0; c < columnCount; ++c)
	{
		int x0 = c * COLUMN_W, x1 = std::min(width, x0 + COLUMN_W);
		angleAt(0.5f * (x0 + x1), colCos[c], colSin[c]);   // centre of the column
	}
}

int Viewport::GetWidth()
{
	return width;
}

int Viewport::GetHeight()
{
	return height;
}

void Viewport::InitFloorBuffer()
{
	Textures::CreateImage(floorImage, width, height, floorBuf.data(), false, false);
}

void Viewport::DrawWalls(const MapArray walls, const Transform viewpoint)
{
	const Image& atlas = Textures::Atlas::GetImage();
	const float T = (float)TILE_SIZE;
	const float px = viewpoint.position.x, py = viewpoint.position.y;
	const float angle = viewpoint.angle * PI_F / 180.0f;
	const float ca = cosf(angle), sa = sinf(angle);

	for (int c = 0; c < columnCount; ++c)
	{
		// Direction of this column's ray: the view direction turned by the column's angle.
		float rdx = ca * colCos[c] - sa * colSin[c];
		float rdy = -(sa * colCos[c] + ca * colSin[c]);      // minus: the map's y axis points down

		// ---- walk the ray from tile to tile until it hits a wall ----
		int mapX = (int)floorf(px / T), mapY = (int)floorf(py / T);
		int stepX = rdx < 0 ? -1 : 1, stepY = rdy < 0 ? -1 : 1;
		float deltaX = rdx != 0 ? fabsf(T / rdx) : 1e30f;    // distance along the ray to cross one whole tile
		float deltaY = rdy != 0 ? fabsf(T / rdy) : 1e30f;
		float sideX = rdx == 0 ? 1e30f : (rdx < 0 ? (px - mapX * T) / -rdx : ((mapX + 1) * T - px) / rdx);   // distance to the next vertical line
		float sideY = rdy == 0 ? 1e30f : (rdy < 0 ? (py - mapY * T) / -rdy : ((mapY + 1) * T - py) / rdy);  // ... horizontal line

		int hitTile = 0;
		bool hitEastWestFace = false;
		float t = 0;                                         // distance along the ray to the wall
		int stepsX = 0, stepsY = 0;
		while (true)
		{
			if (sideX < sideY) { t = sideX; sideX += deltaX; mapX += stepX; ++stepsX; hitEastWestFace = true; }
			else { t = sideY; sideY += deltaY; mapY += stepY; ++stepsY; hitEastWestFace = false; }

			if (stepsX > MAX_RAY_STEPS || stepsY > MAX_RAY_STEPS) { break; }                              // too far away
			if (mapX < 0 || mapY < 0 || mapX >= MAP_TILES_X || mapY >= MAP_TILES_Y) { break; }            // left the map
			int value = walls[mapY * MAP_TILES_X + mapX];
			if (value > 0) { hitTile = value; break; }
		}
		if (hitTile == 0) { continue; }                      // the ray found nothing: no wall in this column

		// ---- where along the wall did it land? (0..1, left to right as the viewer sees it) ----
		float hitX = px + rdx * t, hitY = py + rdy * t;
		float u;
		if (hitEastWestFace) { u = (hitY - floorf(hitY / T) * T) / T; if (rdx < 0) { u = 1 - u; } }
		else { u = (hitX - floorf(hitX / T) * T) / T; if (rdy > 0) { u = 1 - u; } }

		// ---- size on screen ----
		float perp = std::max(t * colCos[c], 0.01f);         // straight-ahead distance: no fisheye
		float lineH = (TILE_SIZE * projDist) / perp;
		float cut = 0;                                       // fraction of the wall off-screen at the top and again at the bottom
		if (lineH > height) { cut = (lineH - height) / (2 * lineH); lineH = (float)height; }
		float top = (height - lineH) / 2;

		int x0 = c * COLUMN_W, x1 = std::min(width, x0 + COLUMN_W);
		float shade = hitEastWestFace ? SHADE_EAST_WEST_FACE : SHADE_NORTH_SOUTH_FACE;

		// ---- draw one single-pixel-wide slice of the texture, stretched over the column ----
		Rect tex = Textures::Atlas::Cell(hitTile - 1);       // map value 1 is atlas texture 0
		float uu = std::min(std::max(u, 0.f), 0.9999f);      // stay inside this texture, never touch its neighbour
		Draw(atlas,
			Rect(origin.x + x0, origin.y + top, (float)(x1 - x0), lineH),
			Rect(tex.x + uu * tex.w, tex.y + cut * tex.h, 0, tex.h * (1 - 2 * cut)),
			Color(shade, shade, shade));
	}
}

void Viewport::DrawFloorAndRoof(const MapArray floor, const MapArray roof, const Transform viewpoint)
{
	if (!floorImage.texture) { InitFloorBuffer(); }

	const Image& atlas = Textures::Atlas::GetImage();
	const unsigned char* atlasPx = atlas.pixels.data();
	const int cell = Textures::Atlas::CellSize();
	const float T = (float)TILE_SIZE;

	// Where each map tile's texture starts inside the atlas image (-1 = no texture). Once per frame
	// instead of once per pixel.
	const int tiles = MAP_TILES_X * MAP_TILES_Y;
	int groundX[tiles], groundY[tiles], ceilingX[tiles], ceilingY[tiles];
	for (int i = 0; i < tiles; ++i)
	{
		Rect g = Textures::Atlas::Cell(floor[i] - 1), r = Textures::Atlas::Cell(roof[i] - 1);
		bool hasAtlas = !atlas.pixels.empty();
		groundX[i] = (floor[i] > 0 && hasAtlas) ? (int)g.x : -1;  groundY[i] = (int)g.y;
		ceilingX[i] = (roof[i] > 0 && hasAtlas) ? (int)r.x : -1;  ceilingY[i] = (int)r.y;
	}

	// This frame's ray direction for every pixel column.
	const float angle = viewpoint.angle * PI_F / 180.0f;
	const float ca = cosf(angle), sa = sinf(angle);
	for (int x = 0; x < width; ++x)
	{
		pxDirX[x] = ca * pxCos[x] - sa * pxSin[x];
		pxDirY[x] = -(sa * pxCos[x] + ca * pxSin[x]);
	}

	auto write = [&](unsigned char* out, int originX, int originY, float u, float v, const unsigned char* fallback)
		{
			if (originX < 0) { out[0] = fallback[0]; out[1] = fallback[1]; out[2] = fallback[2]; }
			else
			{
				int tx = std::min(std::max((int)(u * cell), 0), cell - 1);
				int ty = std::min(std::max((int)(v * cell), 0), cell - 1);
				const unsigned char* src = &atlasPx[((originY + ty) * atlas.width + originX + tx) * 4];
				out[0] = src[0]; out[1] = src[1]; out[2] = src[2];
			}
			out[3] = 255;
		};

	const int half = height / 2;
	for (int y = half; y < height; ++y)                      // the bottom half of the view is floor ...
	{
		float perp = (T / 2.0f) * projDist / (y + 0.5f - half);   // distance straight ahead that this row sees
		int yCeiling = height - 1 - y;                       // ... and the mirrored row in the top half is ceiling

		for (int x = 0; x < width; ++x)
		{
			float t = perp * pxInvCos[x];                    // distance along this column's ray
			float wx = viewpoint.position.x + pxDirX[x] * t;
			float wy = viewpoint.position.y + pxDirY[x] * t;
			int tileX = (int)floorf(wx / T), tileY = (int)floorf(wy / T);

			unsigned char* floorPixel = &floorBuf[((size_t)y * width + x) * 4];
			unsigned char* ceilingPixel = &floorBuf[((size_t)yCeiling * width + x) * 4];

			if (tileX >= 0 && tileX < MAP_TILES_X && tileY >= 0 && tileY < MAP_TILES_Y)
			{
				int i = tileY * MAP_TILES_X + tileX;
				float u = wx / T - tileX, v = wy / T - tileY;
				write(floorPixel, groundX[i], groundY[i], u, v, FLOOR_COLOUR);
				write(ceilingPixel, ceilingX[i], ceilingY[i], u, v, CEILING_COLOUR);
			}
			else
			{
				write(floorPixel, -1, 0, 0, 0, FLOOR_COLOUR);
				write(ceilingPixel, -1, 0, 0, 0, CEILING_COLOUR);
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, floorImage.texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, floorBuf.data());
	Draw(floorImage, Rect(origin.x, origin.y, (float)width, (float)height), Rect(0, 0, (float)width, (float)height));
}

void Viewport::DrawLevel(const Map* map, const Transform viewpoint)
{
	DrawFloorAndRoof(map->GetFloor(), map->GetRoof(), viewpoint);
	DrawWalls(map->GetWalls(), viewpoint);
}

void Viewport::Draw(const Image& image, const Rect& dest, const Rect& source, const Color& tint)
{
	if (image.texture == 0 || image.width <= 0 || image.height <= 0) { return; }

	float u0 = source.x / image.width, u1 = (source.x + source.w) / image.width;
	float v0 = source.y / image.height, v1 = (source.y + source.h) / image.height;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, image.texture);
	glColor4f(tint.r, tint.g, tint.b, tint.a);

	glBegin(GL_QUADS);
	glTexCoord2f(u0, v0); glVertex2f(dest.x, dest.y);
	glTexCoord2f(u1, v0); glVertex2f(dest.x + dest.w, dest.y);
	glTexCoord2f(u1, v1); glVertex2f(dest.x + dest.w, dest.y + dest.h);
	glTexCoord2f(u0, v1); glVertex2f(dest.x, dest.y + dest.h);
	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void Viewport::DrawImage(const Image& image, const Vector2& position, float widthOverride, float heightOverride, const Color& tint)
{
	float w = widthOverride > 0 ? widthOverride : (float)image.width;
	float h = heightOverride > 0 ? heightOverride : (float)image.height;
	Draw(image, Rect(position.x, position.y, w, h), Rect(0, 0, (float)image.width, (float)image.height), tint);
}

void Viewport::DrawTile(int index, const Rect& dest, const Color& tint)
{
	Draw(Textures::Atlas::GetImage(), dest, Textures::Atlas::Cell(index), tint);
}

void Viewport::DrawString(const std::string& text, const Vector2& position, float scale, const Color& tint)
{
	//skip for now
}


