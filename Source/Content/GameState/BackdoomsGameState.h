#pragma once

#include "Architecture/GameState.h"

#include "Content/Save/SaveData.h"
#include "Structs/Event.h"

class Pickup;
class Monster;
class MenuViewport;
class BackdoomsPlayerHandle;

class BackdoomsGameState : public GameState
{
private:
	SaveData save;

	int playerCount;
	int alivePlayers;
	int playersReachedEnd = 0;

	int score = 0;
	int highscore = 0;

	bool isPaused = false;
	bool isStarted = false;
	bool isOver = false;

	std::vector<Pickup*> pickups;
	std::vector<Monster*> monsters;
	std::vector<BackdoomsPlayerHandle*> playerHandles;

	std::vector<int> occupiedRooms;

	MenuViewport* globalViewport;

	Listener onPlayerReachedEnd;
	Listener onPlayerDied;

	void StartGame(int playerCount);
	void SetPlayerBindings(BackdoomsPlayerHandle* playerHandle);

	void PositionPlayers();
	void GenerateExit();
	void GenerateWalls();
	void GenerateGap(int roomX, int roomY, int dir, bool open);
	void GeneratePickups();
	void GenerateMonsters();

	void OnDied();
	void OnEndReached();

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