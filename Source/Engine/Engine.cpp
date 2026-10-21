#include "Engine.h"

#include <GL/freeglut.h>

#include "Renderer.h"
#include "Controller.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"

float Engine::lastTime = 0.0f;
float Engine::deltaTime = 0.0f;
Renderer* Engine::renderer = nullptr;
Controller* Engine::controller = nullptr;
Map* Engine::map = nullptr;
Player* Engine::player = nullptr;

bool Engine::InitEngine()
{
	renderer = new Renderer();

	if (renderer->loadTextures() != 0)
	{
		return false;
	}

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y, 0);

	glutDisplayFunc(Update);
	glutMainLoop();

	Start();

	return true;
}

void Engine::Quit()
{
	glutLeaveMainLoop();
}

void Engine::Start()
{
	controller = new Controller();
	map = new Map();
	player = new Player();

	map->GenerateMap();
	renderer->initFloorBuffer();

	player->SetPosition(Vector2((MAP_TILES_X / 2.f) * TILE_SIZE, (MAP_TILES_Y / 2.f) * TILE_SIZE));
}

void Engine::Update()
{
	updateKeys();

	if (Keys.esc == 1) { Quit(); return; }

	float now = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (now - lastTime) / 1000.f;
	lastTime = now;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	if (Keys.a == 1)
	{
		player->Turn(true);
	}
	if (Keys.d == 1)
	{
		player->Turn(false);
	}

	Vector2 PlayerPosition = player->GetTransform().position;
	Vector2 PlayerForward = player->GetTransform().GetForward();

	int xo = PlayerForward.x < 0 ? -player->GetRadius() : player->GetRadius();
	int yo = PlayerForward.y < 0 ? -player->GetRadius() : player->GetRadius();

	int ipx = PlayerPosition.x / TILE_SIZE, ipx_add_xo = (PlayerPosition.x + xo) / TILE_SIZE, ipx_sub_xo = (PlayerPosition.x - xo) / TILE_SIZE;
	int ipy = PlayerPosition.y / TILE_SIZE, ipy_add_yo = (PlayerPosition.y + yo) / TILE_SIZE, ipy_sub_yo = (PlayerPosition.y - yo) / TILE_SIZE;

	if (Keys.w == 1)
	{
		player->Move(player->GetTransform().GetForward());
	}
	if (Keys.s == 1)
	{
		player->Move(player->GetTransform().GetForward() * -1.f);
	}

	/* OLD CODE
	if (Keys.w == 1)
	{
		if (map->GetWalls()[ipy * MAP_TILES_X + ipx_add_xo] == 0) { PLAYER_X += PLAYER_DELTA_X * PLAYER_SPEED * deltaTime; }
		if (map->GetWalls()[ipy_add_yo * MAP_TILES_X + ipx] == 0) { PLAYER_Y += PLAYER_DELTA_Y * PLAYER_SPEED * deltaTime; }
	}
	if (Keys.s == 1)
	{
		if (map->GetWalls()[ipy * MAP_TILES_X + ipx_sub_xo] == 0) { PLAYER_X -= PLAYER_DELTA_X * PLAYER_SPEED * deltaTime; }
		if (map->GetWalls()[ipy_sub_yo * MAP_TILES_X + ipx] == 0) { PLAYER_Y -= PLAYER_DELTA_Y * PLAYER_SPEED * deltaTime; }
	}*/

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->DrawMap(map, player->GetTransform());
	renderer->drawUI();

	glutSwapBuffers();
}

Player* Engine::GetPlayer()
{
	return player;
}

Map* Engine::GetMap()
{
	return map;
}

float Engine::GetDeltaTime()
{
	return deltaTime;
}
