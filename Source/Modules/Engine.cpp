#include "Engine.h"

#include <GL/freeglut.h>

#include "Textures.h"
#include "Physics.h"
#include "Input.h"

#include "../Architecture/Camera.h"
#include "../Architecture/Level.h"

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

static Level* activeLevel;

static PlayerHUD* MainPlayerHUD;
static PlayerHUD* SecondPlayerHUD;


static void StartGame(bool isMultiplayer)
{
	activeLevel = new Level();

	activeLevel->Spawn<Soda>(activeLevel->GetSpawnpoint() + Vector2(64, 64), 0.f);
	activeLevel->Spawn<Soda>(activeLevel->GetSpawnpoint() + Vector2(-64, 64), 0.f);
	activeLevel->Spawn<Ammo>(activeLevel->GetSpawnpoint() + Vector2(64, -64), 0.f);
	activeLevel->Spawn<Ammo>(activeLevel->GetSpawnpoint() + Vector2(-64, -64), 0.f);

	activeLevel->Spawn<Monster>(Vector2(TILE_SIZE * 10.5f, TILE_SIZE * 2.5f), 0.f);

	Player* MainPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 0.f);
	MainPlayer->AssignController(new PlayerController('W', 'A', 'S', 'D', 'Q'));

	Camera* MainCamera = activeLevel->Spawn<Camera>(activeLevel->GetSpawnpoint(), 0.f, isMultiplayer ? TopScreen : Fullscreen, 60, 20, 2, 1.f);
	MainCamera->SetParent(MainPlayer);

	MainPlayerHUD = new PlayerHUD(MainPlayer);
	
	if (isMultiplayer)
	{
		Player* SecondPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 0.f);
		SecondPlayer->AssignController(new PlayerController(VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT, VK_SPACE));
		
		Camera* SecondCamera = activeLevel->Spawn<Camera>(Vector2(), 0.f, BottomScreen, 60, 20, 2, 1.f);
		SecondCamera->SetParent(SecondPlayer);

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
		for (Object* obj : activeLevel->GetObjects())
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	activeLevel->UpdateObjects();
	Physics::UpdatePhysics();

	glutPostRedisplay();

	if (SecondPlayerHUD != nullptr)
	{
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

	StartGame(false);
	
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glutDisplayFunc(Update);
	glutMainLoop();

	return true;
}

Level* Engine::GetLevel()
{
	return activeLevel;
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
