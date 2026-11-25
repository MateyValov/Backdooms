#include "Engine.h"

#include <GL/freeglut.h>

#include "Window.h"
#include "Textures.h"
#include "Fonts.h"
#include "Physics.h"
#include "Input.h"
#include "Audio.h"

#include "Architecture/Object.h"

#include "Content/Levels/BackdoomsLevel.h"

static float lastTime;
static float deltaTime;
static float timeScale = 1.f;

static Level* activeLevel;


static void Update()
{
	if (activeLevel == nullptr)
	{
		return;
	}

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
	Window::RenderScene(activeLevel);
	Window::RenderViewports(activeLevel);

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

	if (Textures::LoadTextures() == false || Fonts::InitFont("UFO50Tall.ttf") == false || Audio::Init() == false)
	{
		return false;
	}

	Engine::OpenLevel(new BackdoomsLevel());
	
	glClearColor(0, 0, 0, 0);
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	glutDisplayFunc(Update);
	glutMainLoop();

	return true;
}

void Engine::OpenLevel(Level* level)
{
	level->GenerateLevel();
	level->GenerateInitialObjects();
	activeLevel = level;
}

void Engine::Quit()
{
	Fonts::ShutdownFont();
	Audio::Shutdown();
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
