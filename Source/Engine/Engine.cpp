#include "Engine.h"

#include <GL/freeglut.h>

#include "ImageLoader.h"
#include "Renderer.h"
#include "Controller.h"
#include "Viewport.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"

//float Engine::lastTime = 0.0f;
//float Engine::deltaTime = 0.0f;
//Renderer* Engine::renderer = nullptr;
//Controller* Engine::controller = nullptr;
//Map* Engine::map = nullptr;
//Player* Engine::player = nullptr;

bool Engine::InitEngine(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
	glutCreateWindow("Backdooms");

	if (ImageLoader::loadTileset() != 0)
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

	viewport = new Viewport(Vector2(), RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);

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
	if (Keys.w == 1)
	{
		player->Move(player->GetTransform().GetForward());
	}
	if (Keys.s == 1)
	{
		player->Move(player->GetTransform().GetForward() * -1.f);
	}

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewport->DrawLevel(map, player->GetTransform());

	//renderer->DrawMap(map, player->GetTransform());
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
