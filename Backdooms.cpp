#include <iostream>
#include <GL/glut.h>

#include "Source/Engine/Renderer.h"
#include "Source/Engine/Engine.h"


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION_WINDOW_X, RESOLUTION_WINDOW_Y);
	glutCreateWindow("Backdooms");

	if (Engine::Start() != 0)
	{
		return 1;
	}
}
