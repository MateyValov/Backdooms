#include "Engine.h"

#include <GL/freeglut.h>

#include "Textures.h"

#include "../Architecture/Controller.h"
#include "../Architecture/Camera.h"
#include "../Architecture/Player.h"
#include "../Architecture/Map.h"

#include "../Architecture/GUI/MainMenu.h"

static float lastTime;
static float deltaTime;

static int screenWidth;
static int screenHeight;

static Screenspace Fullscreen;
static Screenspace TopScreen;
static Screenspace BottomScreen;

static Map* map;

static Camera* MainViewport;
static Player* MainPlayer;

static Camera* SecondViewport;
static Player* SecondPlayer;

MainMenu* menu;

static void OpenMenu()
{
	menu->DrawWidget(Fullscreen.TopLeft, Fullscreen.BottomRight);
}

static void StartGame(bool isMultiplayer)
{
	menu = new MainMenu();
	map = new Map();

	if (isMultiplayer)
	{
		MainViewport = new Camera(TopScreen, 60, 20, 2);
		MainPlayer = map->Spawn<Player>(map->GetCenter(), 0.f);

		SecondViewport = new Camera(BottomScreen, 60, 20, 2);
		SecondPlayer = map->Spawn<Player>(map->GetCenter(), 0.f);
	}
	else
	{
		MainViewport = new Camera(Fullscreen, 60, 20, 2);
		MainPlayer = map->Spawn<Player>(map->GetCenter(), 0.f);
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
	//OpenMenu();

	glutSwapBuffers();
}

bool Engine::InitEngine(int* argc, char** argv, int screenWidth, int screenHeight)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Backdooms");

	Fullscreen = Screenspace(Vector2(), Vector2(screenWidth, screenHeight));
	TopScreen = Screenspace(Vector2(), Vector2(screenWidth, screenHeight / 2));
	BottomScreen = Screenspace(Vector2(0.f, screenHeight / 2), Vector2(screenWidth, screenHeight));

	if (Textures::LoadTextures() == false)
	{
		return false;
	}

	StartGame(true);
	
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, screenWidth, screenHeight, 0);

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

Vector2 Engine::GetWindowSize()
{
	return Vector2(Fullscreen.x(), Fullscreen.y());
}

Screenspace Engine::GetFullscreenBounds()
{
	return Fullscreen;
}
