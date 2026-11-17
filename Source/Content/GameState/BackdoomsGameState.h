#pragma once

#include "Architecture/GameState.h"

class Pickup;
class Monster;

class BackdoomsGameState : public GameState
{
private:
	int playerCount = 1;
	bool isPaused = false;
	bool isStarted = true;

	std::vector<Pickup*> pickups;
	std::vector<Monster*> monsters;

	void GenerateExit();
	void GenerateWalls();
	void GeneratePickups(int perTypeAmount);
	void GenerateMonsters();

	void EscPressed();
	void EnterPressed();

protected:
	virtual void OnStart() override;
	virtual void OnUpdate() override;

public:
	BackdoomsGameState();

	void SetupLevel();

	bool IsValidRoomId(int id);
	Vector2 TileCoordinatesToRealCoordinates(Vector2 in);
	Vector2 GetRoomCoordinates(int roomId);
};