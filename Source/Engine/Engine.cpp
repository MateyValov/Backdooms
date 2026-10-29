#include "Engine.h"

#include <GL/freeglut.h>

#include "Textures.h"
#include "Controller.h"
#include "Viewport.h"

#include "../Objects/Player.h"
#include "../Objects/Map.h"

#include "../Structs/Window.h"

static float lastTime;
static float deltaTime;

static Map* map;

static Viewport* MainViewport;
static Player* MainPlayer;

static Viewport* SecondViewport;
static Player* SecondPlayer;


static void Start(bool isMultiplayer)
{
	map = new Map();

	MainViewport = new Viewport(Vector2(), Window::x, Window::y / (isMultiplayer ? 2 : 1));
	MainPlayer = map->Spawn<Player>(map->GetCenter(), 0.f);

	if (isMultiplayer)
	{
		SecondViewport = new Viewport(Vector2(0.f, Window::y / 2), Window::x, Window::y / 2);
		SecondPlayer = map->Spawn<Player>(map->GetCenter(), 0.f);
	}
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
	if (Keys.q == 1)
	{
		MainPlayer->InteractPressed();
	}

	if (Keys.left == 1 && SecondPlayer != nullptr)
	{
		SecondPlayer->Turn(true);
	}
	if (Keys.right == 1 && SecondPlayer != nullptr)
	{
		SecondPlayer->Turn(false);
	}
	if (Keys.up == 1 && SecondPlayer != nullptr)
	{
		SecondPlayer->Move(SecondPlayer->GetTransform().GetForward());
	}
	if (Keys.down == 1 && SecondPlayer != nullptr)
	{
		SecondPlayer->Move(SecondPlayer->GetTransform().GetForward() * -1.f);
	}
	if (Keys.space == 1)
	{
		SecondPlayer->InteractPressed();
	}

	glutPostRedisplay();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MainViewport->DrawLevel(map, MainPlayer->GetTransform());
	if (SecondViewport != nullptr && SecondPlayer != nullptr)
	{
		SecondViewport->DrawLevel(map, SecondPlayer->GetTransform());
	}

	glutSwapBuffers();
}

bool Engine::InitEngine(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Window::x, Window::y);
	glutCreateWindow("Backdooms");

	if (Textures::LoadTextures() == false)
	{
		return false;
	}

	Start(true);

	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, Window::x, Window::y, 0);

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
