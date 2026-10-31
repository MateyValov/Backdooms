#include "Source/Modules/Engine.h"


int main(int argc, char* argv[])
{
	if (Engine::InitEngine(&argc, argv, 640, 480) != 0)
	{
		return 1;
	}
}
