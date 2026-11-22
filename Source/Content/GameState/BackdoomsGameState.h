#pragma once

#include "Architecture/GameState.h"

class Pickup;
class Monster;
class BackdoomsPlayerHandle;

class BackdoomsGameState : public GameState
{
private:
	int playerCount;
	bool isPaused = false;
	bool isStarted = false;

	std::vector<Pickup*> pickups;
	std::vector<Monster*> monsters;
	std::vector<BackdoomsPlayerHandle*> playerHandles;

	std::vector<int> occupiedRooms;

	void StartGame(int playerCount);

	void PositionPlayers();
	void GenerateExit();
	void GenerateWalls();
	void GeneratePickups();
	void GenerateMonsters();

	void EscPressed();
	void EnterPressed();

protected:
	virtual void OnStart() override;
	virtual void OnUpdate() override;

public:
	BackdoomsGameState() = default;

	void SetupLevel();

	bool IsValidRoomId(int id);
	Vector2 TileCoordinatesToRealCoordinates(Vector2 in);

	Vector2 GetRoomCoordinates(int roomId);
	Vector2 GetRoomCornerCoordinates(int roomId);
};