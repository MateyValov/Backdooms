#include "Engine.h"

#include <GL/freeglut.h>

#include "ImageLoader.h"
#include "Controller.h"
#include "Viewport.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"

static float lastTime;
static float deltaTime;

static Controller* controller;
static Map* map;

static Viewport* MainViewport;
static Player* MainPlayer;


static void Start()
{
	controller = new Controller();
	map = new Map();
	map->GenerateMap();

	MainViewport = new Viewport(Vector2(), RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);

	MainPlayer = new Player();
	MainPlayer->SetPosition(Vector2((MAP_TILES_X / 2.f) * TILE_SIZE, (MAP_TILES_Y / 2.f) * TILE_SIZE));
}

static void Quit()
{
	glutLeaveMainLoop();
}

static void Update()
{
	float now = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (now - lastTime) / 1000.f;
	lastTime = now;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	updateKeys();

	if (Keys.esc == 1) 
	{ 
		Quit(); 
		return;
	}
	if (Keys.a == 1)
	{
		MainPlayer->Turn(true);
	}
	if (Keys.d == 1)
	{
		MainPlayer->Turn(false);
	}
	if (Keys.w == 1)
	{
		MainPlayer->Move(MainPlayer->GetTransform().GetForward());
	}
	if (Keys.s == 1)
	{
		MainPlayer->Move(MainPlayer->GetTransform().GetForward() * -1.f);
	}

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MainViewport->DrawLevel(map, MainPlayer->GetTransform());

	glutSwapBuffers();
}

bool Engine::InitEngine(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
	glutCreateWindow("Backdooms");

	if (Textures::LoadTextures() == false)
	{
		return false;
	}

	Start();

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y, 0);

	glutDisplayFunc(Update);
	glutMainLoop();

	return true;
}

Player* Engine::GetMainPlayer()
{
	return MainPlayer;
}

Map* Engine::GetMap()
{
	return map;
}

float Engine::GetDeltaTime()
{
	return deltaTime;
}
