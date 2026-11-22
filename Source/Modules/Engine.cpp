#include "Engine.h"

#include <GL/freeglut.h>

#include "Window.h"
#include "Textures.h"
#include "Fonts.h"
#include "Physics.h"
#include "Input.h"

#include "Architecture/Camera.h"

//======================================

#include "Content/Levels/BackdoomsLevel.h"

#include "Content/Objects/Player.h"

#include "Content/GUI/PlayerHUD.h"
#include "Content/GUI/MainMenu.h"

static float lastTime;
static float deltaTime;
static float timeScale = 1.f;

static Level* activeLevel;

//static PlayerHUD* MainPlayerHUD;
//static PlayerHUD* SecondPlayerHUD;
//static MainMenu* m;


static void OpenLevel(Level* level, bool isMultiplayer)
{
	level->GenerateLevel();
	level->GenerateInitialObjects();
	activeLevel = level;

	/*Player* MainPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 90.f);
	MainPlayer->AssignController(new PlayerController(ControlScheme::PlayerOne()));

	Camera* MainCamera = activeLevel->Spawn<Camera>(activeLevel->GetSpawnpoint(), 0.f, 
		isMultiplayer ? Window::TopHalf() : Window::Fullscreen(), 60, 20, 2, 1.f);
	MainCamera->SetParent(MainPlayer);

	MainPlayerHUD = new PlayerHUD(MainPlayer);
	m = new MainMenu();
	
	if (isMultiplayer)
	{
		Player* SecondPlayer = activeLevel->Spawn<Player>(activeLevel->GetSpawnpoint(), 90.f);
		SecondPlayer->AssignController(new PlayerController(ControlScheme::PlayerTwo()));
		
		Camera* SecondCamera = activeLevel->Spawn<Camera>(Vector2(), 0.f, Window::BottomHalf(), 60, 20, 2, 1.f);
		SecondCamera->SetParent(SecondPlayer);

		SecondPlayerHUD = new PlayerHUD(SecondPlayer);
	}*/
}

static void Update()
{
	float now = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = (now - lastTime) / 1000.f;
	lastTime = now;
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }

	Input::updateKeys();
	if (Input::GetKeyState('I') == pressed)
	{
		printf("========OBJECTS IN MAP========\n");
		for (Object* obj : activeLevel->GetObjects())
		{
			printf("Object: x - %.3f, y - %.3f, forward vector - %.3f, %.3f\n",
				obj->GetTransform().position.x, obj->GetTransform().position.y, obj->GetTransform().GetForward().x, obj->GetTransform().GetForward().y);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	activeLevel->UpdateObjects();
	Physics::UpdatePhysics(activeLevel);
	activeLevel->RemoveDestroyed();

	glutPostRedisplay();

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

	if (Textures::LoadTextures() == false || Fonts::InitFont("UFO50Tall.ttf") == false)
	{
		return false;
	}

	OpenLevel(new BackdoomsLevel(), true);
	
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glutDisplayFunc(Update);
	glutMainLoop();

	return true;
}

void Engine::Quit()
{
	Fonts::ShutdownFont();
	glutLeaveMainLoop();
}

float Engine::GetDeltaTime()
{
	return deltaTime * timeScale;
}

float Engine::GetTimeScale()
{
	return timeScale;
}

void Engine::SetTimeScale(float value)
{
	timeScale = value;
}
