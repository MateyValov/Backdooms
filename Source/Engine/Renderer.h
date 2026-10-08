#pragma once

#include "ImageLoader.h"

#define PI 3.141593

#define RESOLUTION_WINDOW_X 640 
#define RESOLUTION_WINDOW_Y 480

#define MAX_RAY_STEPS 20

#define BOTTOM_MARGIN 128                               // free space under the 3D view
#define VIEW_W RESOLUTION_WINDOW_X                         // 3D view is as wide as the window
#define VIEW_H (RESOLUTION_WINDOW_Y - BOTTOM_MARGIN)       // 412; keep this number even

#define FOV_DEGREES 60                                     // real field of view, in degrees
#define COLUMN_W 4                                         // pixels per ray; must divide VIEW_W exactly
#define RAY_COUNT (VIEW_W / COLUMN_W)                      // 256 rays
#define RAY_STEP ((float)FOV_DEGREES / RAY_COUNT)          // degrees between neighbouring rays

#define PROJ_DIST VIEW_H   // how tall walls look: a wall one tile away fills the view height

#define PROGRESS_BAR_OFFSET_V 106
#define HEALTH_BAR_OFFSET_H 20
#define AMMO_BAR_OFFSET_H 78

#define PLAYER_ICON_OFFSET_V 8
#define PLAYER_ICON_OFFSET_H 264

extern unsigned char floorBuf[VIEW_W * VIEW_H * 4];
extern GLuint floorBufTex;

extern Image gui;
extern Image overlay;
extern Image healthBar;
extern Image ammoBar;
extern Image PlayerIcon;

float dist(float ax, float ay, float bx, float by, float ang);

float degToRad(float a);

float FixAng(float a);

void drawPlayer();

void drawMap();

void drawWalls();

void sampleTile(unsigned char* out, int tile, float u, float v, const unsigned char* fallback);

void buildFloorAndCeiling();

void initFloorBuffer();

void drawFloorAndCeiling();
