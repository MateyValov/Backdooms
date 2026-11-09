#include "Engine.h"

#include <GL/freeglut.h>

#include "Window.h"
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

static Level* activeLevel;

static PlayerHUD* MainPlayerHUD;
static PlayerHUD* SecondPlayerHUD;


static void OpenLevel(Level* level, bool isMultiplayer)
{
	activeLevel = level;

	activeLevel->Spawn<Soda>(activeLevel->GetSpawnpoint() + Vector2(64, 64), 0.f, 10);
	activeLevel->Spawn<Soda>(activeLevel->GetSpawnpoint() + Vector2(-64, 64), 0.f, 10);
	activeLevel->Spawn<Ammo>(activeLevel->GetSpawnpoint() + Vector2(64, -64), 0.f, 10);
	activeLevel->Spawn<Ammo>(activeLevel->GetSpawnpoint() + Vector2(-64, -64), 0.f, 10);

	activeLevel->Spawn<Monster>(Vector2(TILE_SIZE * 10.5f, TILE_SIZE * 2.5f), 0.f);

	Player* MainPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 0.f);
	MainPlayer->AssignController(new PlayerController(ControlScheme::PlayerOne()));

	Camera* MainCamera = activeLevel->Spawn<Camera>(activeLevel->GetSpawnpoint(), 0.f, 
		isMultiplayer ? Window::TopHalf() : Window::Fullscreen(), 60, 20, 2, 1.f);
	MainCamera->SetParent(MainPlayer);

	MainPlayerHUD = new PlayerHUD(MainPlayer);
	
	if (isMultiplayer)
	{
		Player* SecondPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 0.f);
		SecondPlayer->AssignController(new PlayerController(ControlScheme::PlayerTwo()));
		
		Camera* SecondCamera = activeLevel->Spawn<Camera>(Vector2(), 0.f, Window::BottomHalf(), 60, 20, 2, 1.f);
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
		SecondPlayerHUD->DrawWidget(Window::BottomHalf());
		MainPlayerHUD->DrawWidget(Window::TopHalf());
	}
	else
	{
		MainPlayerHUD->DrawWidget(Window::Fullscreen());
	}

	glutSwapBuffers();
}

bool Engine::InitEngine(int* argc, char** argv, int screenWidth, int screenHeight)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow("Backdooms");

	Window::InitWindow(screenWidth, screenHeight);
	Input::InitKeyStates();

	if (Textures::LoadTextures() == false)
	{
		return false;
	}

	OpenLevel(new Level(), true);
	
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
