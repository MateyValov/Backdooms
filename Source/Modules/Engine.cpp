#include "Engine.h"

#include <GL/freeglut.h>

#include "Textures.h"

#include "../Architecture/Controller.h"
#include "../Architecture/Camera.h"
#include "../Architecture/Player.h"
#include "../Architecture/Map.h"

//======================================

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


static void OpenMenu()
{
	//idk
}

static void StartGame(bool isMultiplayer)
{
	map = new Map();

	map->Spawn<Soda>(map->GetSpawnpoint() + Vector2(64, 64), 0.f);
	map->Spawn<Soda>(map->GetSpawnpoint() + Vector2(-64, 64), 0.f);
	map->Spawn<Ammo>(map->GetSpawnpoint() + Vector2(64, -64), 0.f);
	map->Spawn<Ammo>(map->GetSpawnpoint() + Vector2(-64, -64), 0.f);

	map->Spawn<Monster>(Vector2(TILE_SIZE * 10 + TILE_SIZE / 2, TILE_SIZE * 2 + TILE_SIZE / 2), 0.f);

	if (isMultiplayer)
	{
		MainViewport = new Camera(TopScreen, 60, 20, 2, 5.f);
		MainPlayer = map->Spawn<Player>(map->GetSpawnpoint(), 0.f);
		MainPlayerHUD = new PlayerHUD(MainPlayer);

		SecondViewport = new Camera(BottomScreen, 60, 20, 2, 1.f);
		SecondPlayer = map->Spawn<Player>(map->GetSpawnpoint(), 0.f);
		SecondPlayerHUD = new PlayerHUD(SecondPlayer);
	}
	else
	{
		MainViewport = new Camera(Fullscreen, 60, 20, 2, 5.f);
		MainPlayer = map->Spawn<Player>(map->GetSpawnpoint(), 0.f);
		MainPlayerHUD = new PlayerHUD(MainPlayer);
	}
}

static void Quit()
{
	glutLeaveMainLoop();
}

static void HandleInput()
{
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

	if (Keys.i == 1)
	{
		printf("Player 1: Health - %d, Ammo - %d\n", MainPlayer->GetHealth(), MainPlayer->GetAmmo());
		if (SecondPlayer != nullptr)
		{
			printf("Player 2: Health - %d, Ammo - %d\n", SecondPlayer->GetHealth(), SecondPlayer->GetAmmo());
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
