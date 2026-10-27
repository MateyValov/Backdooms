#pragma once

class Map;
class Player;
class Renderer;
class Controller;

class Viewport;

class Engine
{
private:
	static float lastTime; 
	static float deltaTime;

	static Controller* controller;
	static Player* player;
	static Map* map;

	static Viewport* viewport;

	static void Start();
	static void Update();
	static void Quit();

public:
	static bool InitEngine(int* argc, char** argv);

	static Player* GetPlayer();
	static Map* GetMap();

	static float GetDeltaTime();
};