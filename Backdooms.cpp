#include <iostream>
#include <GL/glut.h>

#include "Source/Engine/Renderer.h"
#include "Source/Engine/Engine.h"


int main(int argc, char* argv[])
{
	if (Engine::InitEngine(&argc, argv) != 0)
	{
		return 1;
	}
}
