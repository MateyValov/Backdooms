#include "Engine.h"

#include <GL/freeglut.h>

#include "Textures.h"
#include "Input.h"

#include "../Architecture/Camera.h"
#include "../Architecture/Map.h"

//======================================

#include "../Content/Objects/Player.h"
#include "../Content/Input/PlayerController.h"

#include "../Content/Objects/Soda.h"
#include "../Content/Objects/Ammo.h"
#include "../Content/Objects/Monster.h"

#include "../Content/GUI/PlayerHUD.h"

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

static PlayerHUD* MainPlayerHUD;
static PlayerHUD* SecondPlayerHUD;


static void StartGame(bool isMultiplayer)
{
	map = new Map();

	map->Spawn<Soda>(map->GetSpawnpoint() + Vector2(64, 64), 0.f);
	map->Spawn<Soda>(map->GetSpawnpoint() + Vector2(-64, 64), 0.f);
	map->Spawn<Ammo>(map->GetSpawnpoint() + Vector2(64, -64), 0.f);
	map->Spawn<Ammo>(map->GetSpawnpoint() + Vector2(-64, -64), 0.f);

	map->Spawn<Monster>(Vector2(TILE_SIZE * 10.5f, TILE_SIZE * 2.5f), 0.f);

	MainPlayer = map->Spawn<Player>(map->GetSpawnpoint(), 0.f);
	MainPlayer->AssignController(new PlayerController('W', 'A', 'S', 'D', 'Q'));

	MainPlayerHUD = new PlayerHUD(MainPlayer);
	MainViewport = new Camera(isMultiplayer ? TopScreen : Fullscreen, 60, 20, 2, 1.f);
	
	if (isMultiplayer)
	{
		SecondPlayer = map->Spawn<Player>(map->GetSpawnpoint(), 0.f);
		SecondPlayer->AssignController(new PlayerController(VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_SPACE));
		
		SecondViewport = new Camera(BottomScreen, 60, 20, 2, 1.f);
		SecondPlayerHUD = new PlayerHUD(SecondPlayer);
	}
}

static void Quit()
{
	glutLeaveMainLoop();
}

static void HandleInput()
{
	Input::updateKeys();

	if (Input::GetKeyState(VK_ESCAPE) == pressed)
	{
		Quit();
	}
	if (Input::GetKeyState('I') == pressed)
	{
		printf("========OBJECTS IN MAP========\n");
		for (Object* obj : map->GetObjects())
		{
			printf("Object: x - %.3f, y - %.3f, forward vector - %.3f, %.3f\n", 
				obj->GetTransform().position.x, obj->GetTransform().position.y, obj->GetTransform().GetForward().x, obj->GetTransform().GetForward().y);
		}
	}
}

static void Update()
{
	float now = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (now - lastTime) / 1000.f;
	lastTime = now;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	HandleInput();
	map->UpdateObjects();
	
	glutPostRedisplay();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	MainViewport->DrawLevel(map, MainPlayer->GetTransform());
	
	if (SecondViewport != nullptr && SecondPlayer != nullptr)
	{
		SecondViewport->DrawLevel(map, SecondPlayer->GetTransform());
		SecondPlayerHUD->DrawWidget(BottomScreen);
		MainPlayerHUD->DrawWidget(TopScreen);
	}
	else
	{
		MainPlayerHUD->DrawWidget(Fullscreen);
	}
	

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

	Input::InitKeyStates();
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
