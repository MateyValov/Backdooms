#pragma once

class Map;
class Player;
class Renderer;
class Controller;

static class Engine
{
private:
	static float lastTime; 
	static float deltaTime;

	static Renderer* renderer;
	static Controller* controller;
	static Player* player;
	static Map* map;

	static void Start();
	static void Update();
	static void Quit();

public:
	static bool InitEngine();

	static Player* GetPlayer();
	static Map* GetMap();

	static float GetDeltaTime() const;
};