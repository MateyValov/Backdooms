#include "Camera.h"

#include <algorithm>
#include <cmath>

#include "Components/Sprite.h"
#include "Level.h"  

#include "../Modules/Rendering.h"
#include "../Modules/Textures.h"
#include "../Modules/Engine.h"

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

namespace
{
	const float SHADE_EAST_WEST_FACE = 0.9f;     
	const float SHADE_NORTH_SOUTH_FACE = 0.7f;

	const unsigned char FLOOR_COLOUR[3] = { 40, 40, 40 };
	const unsigned char CEILING_COLOUR[3] = { 60, 60, 75 };
}

Camera::Camera()
{
	screenspace = Window::Fullscreen();
	Setup();
}

Camera::Camera(Screenspace screenspace, int fov, int maxRaySteps, int columnWidth, float nearClippingDistance)
{
	this->screenspace = screenspace;
	this->fov = fov;
	this->MaxRaySteps = maxRaySteps;
	this->columnWidth = columnWidth;
	this->nearClippingDistance = nearClippingDistance;
	Setup();
}

void Camera::Setup()
{
	columnCount = (GetWidth() + columnWidth - 1) / columnWidth;
	projDist = (float)GetHeight();

	wallDepth.assign(columnCount, FLT_MAX);
	floorBuf.assign((size_t)GetWidth() * GetHeight() * 4, 0);

	auto angleAt = [&](float xf, float& c, float& s)
		{
			float theta = (fov / 2.0f - xf * ((float)fov / GetWidth())) * PI / 180.0f;
			c = cosf(theta);
			s = sinf(theta);
		};

	pxCos.resize(GetWidth()); pxSin.resize(GetWidth()); pxInvCos.resize(GetWidth());
	pxDirX.resize(GetWidth()); pxDirY.resize(GetWidth());
	for (int x = 0; x < GetWidth(); ++x)
	{
		angleAt(x + 0.5f, pxCos[x], pxSin[x]); 
		pxInvCos[x] = 1.0f / pxCos[x];
	}

	colCos.resize(columnCount); colSin.resize(columnCount);
	for (int c = 0; c < columnCount; ++c)
	{
		int x0 = c * columnWidth, x1 = min(GetWidth(), x0 + columnWidth);
		angleAt(0.5f * (x0 + x1), colCos[c], colSin[c]);
	}
}

int Camera::GetWidth()
{
	return screenspace.x();
}

int Camera::GetHeight()
{
	return screenspace.y();
}

Screenspace Camera::GetScreenspace()
{
	return screenspace;
}

void Camera::InitFloorBuffer()
{
	Textures::CreateImage(floorImage, GetWidth(), GetHeight(), floorBuf.data(), false, false);
}

void Camera::DrawWalls(const MapArray walls)
{
	const Image& atlas = Textures::Atlas::GetImage();
	const float T = (float)TILE_SIZE;
	const float px = transform.position.x, py = transform.position.y;
	const float angle = transform.angle * PI / 180.0f;
	const float ca = cosf(angle), sa = sinf(angle);

	fill(wallDepth.begin(), wallDepth.end(), FLT_MAX);

	for (int c = 0; c < columnCount; ++c)
	{
		float rdx = ca * colCos[c] - sa * colSin[c];
		float rdy = -(sa * colCos[c] + ca * colSin[c]);

		int mapX = (int)floorf(px / T), mapY = (int)floorf(py / T);
		int stepX = rdx < 0 ? -1 : 1, stepY = rdy < 0 ? -1 : 1;
		float deltaX = rdx != 0 ? fabsf(T / rdx) : 1e30f;   
		float deltaY = rdy != 0 ? fabsf(T / rdy) : 1e30f;
		float sideX = rdx == 0 ? 1e30f : (rdx < 0 ? (px - mapX * T) / -rdx : ((mapX + 1) * T - px) / rdx);   
		float sideY = rdy == 0 ? 1e30f : (rdy < 0 ? (py - mapY * T) / -rdy : ((mapY + 1) * T - py) / rdy);  

		int hitTile = 0;
		bool hitEastWestFace = false;
		float t = 0;                                        
		int stepsX = 0, stepsY = 0;
		while (true)
		{
			if (sideX < sideY) { t = sideX; sideX += deltaX; mapX += stepX; ++stepsX; hitEastWestFace = true; }
			else { t = sideY; sideY += deltaY; mapY += stepY; ++stepsY; hitEastWestFace = false; }

			if (stepsX > MaxRaySteps || stepsY > MaxRaySteps) { break; }                              
			if (mapX < 0 || mapY < 0 || mapX >= MAP_TILES_X || mapY >= MAP_TILES_Y) { break; }           
			int value = walls[mapY * MAP_TILES_X + mapX];
			if (value > 0) { hitTile = value; break; }
		}
		if (hitTile == 0) { continue; }                      

		float hitX = px + rdx * t, hitY = py + rdy * t;
		float u;
		if (hitEastWestFace) { u = (hitY - floorf(hitY / T) * T) / T; if (rdx < 0) { u = 1 - u; } }
		else { u = (hitX - floorf(hitX / T) * T) / T; if (rdy > 0) { u = 1 - u; } }


		float perp = std::max(t * colCos[c], 0.01f);         
		wallDepth[c] = perp;                                
		float lineH = (TILE_SIZE * projDist) / perp;
		float cut = 0;                                       
		if (lineH > GetHeight()) { cut = (lineH - GetHeight()) / (2 * lineH); lineH = (float)GetHeight(); }
		float top = (GetHeight() - lineH) / 2;

		int x0 = c * columnWidth, x1 = min(GetWidth(), x0 + columnWidth);
		float shade = hitEastWestFace ? SHADE_EAST_WEST_FACE : SHADE_NORTH_SOUTH_FACE;

		Rect tex = Textures::Atlas::Cell(hitTile - 1);
		float uu = std::min(std::max(u, 0.f), 0.9999f);
		Rendering::Draw(atlas,
			Rect(screenspace.TopLeft.x + x0, screenspace.TopLeft.y + top, (float)(x1 - x0), lineH),
			Rect(tex.x + uu * tex.w, tex.y + cut * tex.h, 0, tex.h * (1 - 2 * cut)),
			Color(shade, shade, shade));
	}
}

void Camera::DrawFloorAndRoof(const MapArray floor, const MapArray roof)
{
	if (!floorImage.texture) 
	{ 
		InitFloorBuffer(); 
	}

	const Image& atlas = Textures::Atlas::GetImage();
	const unsigned char* atlasPx = atlas.pixels.data();
	const int cell = Textures::Atlas::CellSize();
	const float T = (float)TILE_SIZE;

	const int tiles = MAP_TILES_X * MAP_TILES_Y;
	int groundX[tiles], groundY[tiles], ceilingX[tiles], ceilingY[tiles];
	for (int i = 0; i < tiles; ++i)
	{
		Rect g = Textures::Atlas::Cell(floor[i] - 1), r = Textures::Atlas::Cell(roof[i] - 1);
		bool hasAtlas = !atlas.pixels.empty();
		groundX[i] = (floor[i] > 0 && hasAtlas) ? (int)g.x : -1;  groundY[i] = (int)g.y;
		ceilingX[i] = (roof[i] > 0 && hasAtlas) ? (int)r.x : -1;  ceilingY[i] = (int)r.y;
	}

	const float angle = transform.angle * PI / 180.0f;
	const float ca = cosf(angle), sa = sinf(angle);
	for (int x = 0; x < GetWidth(); ++x)
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

	const int half = GetHeight() / 2;
	for (int y = half; y < GetHeight(); ++y)                      
	{
		float perp = (T / 2.0f) * projDist / (y + 0.5f - half); 
		int yCeiling = GetHeight() - 1 - y;                 

		for (int x = 0; x < GetWidth(); ++x)
		{
			float t = perp * pxInvCos[x];                   
			float wx = transform.position.x + pxDirX[x] * t;
			float wy = transform.position.y + pxDirY[x] * t;
			int tileX = (int)floorf(wx / T), tileY = (int)floorf(wy / T);

			unsigned char* floorPixel = &floorBuf[((size_t)y * GetWidth() + x) * 4];
			unsigned char* ceilingPixel = &floorBuf[((size_t)yCeiling * GetWidth() + x) * 4];

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
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GetWidth(), GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, floorBuf.data());
	Rendering::Draw(
		floorImage, 
		Rect(screenspace.TopLeft.x, screenspace.TopLeft.y, (float)GetWidth(), (float)GetHeight()), 
		Rect(0, 0, (float)GetWidth(), (float)GetHeight()));
}

void Camera::DrawObjects(const vector<Object*>& objects)
{
	const Image& atlas = Textures::Atlas::GetImage();
	const float fx = transform.GetForward().x, fy = transform.GetForward().y;
	const float rx = transform.GetRight().x, ry = transform.GetRight().y;

	struct Visible { Sprite* sprite; float depth; float lateral; int texture; };
	vector<Visible> visible;
	for (Object* object : objects)
	{
		vector<Sprite*> sprites = object->GetAllComponentOfClass<Sprite>();
		if (sprites.size() == 0)
		{
			continue;
		}
		for (Sprite* s : sprites)
		{
			int texture = s->GetTextureIdForPosition(transform.position);
			if (texture < 0)
			{
				continue;
			}

			Vector2 p = object->GetTransform().position;
			float dx = p.x - transform.position.x, dy = p.y - transform.position.y;
			float depth = dx * fx + dy * fy;
			if (depth < nearClippingDistance)
			{
				continue;
			}

			visible.push_back(Visible{ s, depth, dx * rx + dy * ry, texture });
		}
	}
	sort(visible.begin(), visible.end(), [](const Visible& a, const Visible& b) { return a.depth > b.depth; }); 

	for (const Visible& v : visible)
	{
		float scale = projDist / v.depth;
		float size = TILE_SIZE * scale;
		float pixelsPerDegree = (float)GetWidth() / fov;

		float left = (fov / 2.0f + atan2f(v.lateral - TILE_SIZE / 2, v.depth) * 180.0f / PI) * pixelsPerDegree;
		float right = (fov / 2.0f + atan2f(v.lateral + TILE_SIZE / 2, v.depth) * 180.0f / PI) * pixelsPerDegree;
		float bottom = GetHeight() / 2.0f + (TILE_SIZE / 2.0f) * scale;
		float top = bottom - size;

		float visibleTop = max(top, 0.0f), visibleBottom = min(bottom, (float)GetHeight()); 
		if (visibleBottom <= visibleTop || right <= 0 || left >= GetWidth())
		{ 
			continue; 
		}
		float v0 = (visibleTop - top) / size, v1 = (visibleBottom - top) / size;   

		Rect tex = Textures::Atlas::Cell(v.texture);
		int firstColumn = max(0, (int)floorf(left / columnWidth));
		int lastColumn = min(columnCount - 1, (int)floorf((right - 0.001f) / columnWidth));
		for (int c = firstColumn; c <= lastColumn; ++c)
		{
			if (v.depth >= wallDepth[c]) 
			{ 
				continue; 
			}            

			float x0 = max((float)(c * columnWidth), left), x1 = min((float)min(GetWidth(), (c + 1) * columnWidth), right);
			if (x1 <= x0) 
			{ 
				continue; 
			}
			float direction = (0.5f * (x0 + x1) / pixelsPerDegree - fov / 2.0f) * PI / 180.0f;
			float u = min(max((v.depth * tanf(direction) - (v.lateral - TILE_SIZE / 2)) / TILE_SIZE, 0.0f), 0.9999f);

			Rendering::Draw(atlas,
				Rect(screenspace.TopLeft.x + x0, screenspace.TopLeft.y + visibleTop, x1 - x0, visibleBottom - visibleTop),
				Rect(tex.x + u * tex.w, tex.y + v0 * tex.h, 0, (v1 - v0) * tex.h),
				Color());
		}
	}
}

void Camera::OnStart()
{
	printf("========CAMERA STATS========\n");
	printf("Viewport - %d/%d\n", GetWidth(), GetHeight());
	printf("MaxRaySteps = %d\n", MaxRaySteps);
	printf("fov = %d\n", fov);
	printf("columnWidth = %d\n", columnWidth);
	printf("nearClippingDistance = %.2f\n", nearClippingDistance);
	printf("columnCount = %d\n", columnCount);
	printf("projDist = %.2f\n", projDist);
}

void Camera::OnUpdate()
{
	Level* level = Engine::GetLevel();

	DrawFloorAndRoof(level->GetFloor(), level->GetRoof());
	DrawWalls(level->GetWalls());
	DrawObjects(level->GetObjects());
}




