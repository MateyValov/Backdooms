#include "BackdoomsGameState.h"

#include <stdlib.h>
#include <time.h>

#include "Modules/Input.h"
#include "Modules/Engine.h"
#include "Modules/Window.h"

#include "Content/Objects/Soda.h"
#include "Content/Objects/Ammo.h"
#include "Content/Objects/Player.h"
#include "Content/Objects/Monster.h"
#include "Content/Levels/BackdoomsLevel.h"
#include "Content/Player/BackdoomsPlayerHandle.h"
#include "Content/Components/HittableComponent.h"
#include "Content/GUI/MenuViewport.h"
#include "Content/GUI/ScoreWidget.h"

namespace
{
	const int ROOMS_X = 5;
	const int ROOMS_Y = 5;
	const int ROOM_STRIDE = 4;
	const int EXIT_OFFSET = 2;
	const int EXTRA_OPEN_PERCENT = 25;
}

void BackdoomsGameState::OnStart()
{
	globalViewport = dynamic_cast<MenuViewport*>(GetLevel()->GetGlobalViewport());
	save.InitSaveData("Backdooms", "save.bds");
	save.Load();
}

void BackdoomsGameState::OnUpdate()
{
	if (Input::GetKeyState('1') == pressed && isStarted == false)
	{
		StartGame(1);
	}
	if (Input::GetKeyState('2') == pressed && isStarted == false)
	{
		StartGame(2);
	}
	if (Input::GetKeyState(0x1B) == pressed) //escape
	{
		EscPressed();
	}
	if (Input::GetKeyState(0x0D) == pressed) //return
	{
		EnterPressed();
	}
}

void BackdoomsGameState::StartGame(int playerCount)
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	size_t n = playerHandles.size();
	for (size_t i = 0; i < n; i++)
	{
		if (IsValid(playerHandles[i]) == false)
		{
			continue;
		}
		playerHandles[i]->Destroy();
	}
	playerHandles.clear();

	onPlayerReachedEnd.Unsubscribe();
	onPlayerDied.Unsubscribe();

	isStarted = true;
	this->playerCount = playerCount;
	alivePlayers = playerCount;

	if (playerCount == 2)
	{
		BackdoomsPlayerHandle* playerOne = GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::TopHalf(), ControlScheme::PlayerOne());
		SetPlayerBindings(playerOne);
		playerHandles.push_back(playerOne);

		BackdoomsPlayerHandle* playerTwo = GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::BottomHalf(), ControlScheme::PlayerTwo());
		SetPlayerBindings(playerTwo);
		playerHandles.push_back(playerTwo);
	}
	else
	{
		BackdoomsPlayerHandle* player = GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::Fullscreen(), ControlScheme::PlayerOne());
		SetPlayerBindings(player);
		playerHandles.push_back(player);
	}

	SetupLevel();

	score = 0;
	highscore = playerCount == 2 ? save.GetMultiplayerHighscore() : save.GetSingleplayerHighscore();

	if (globalViewport == nullptr)
	{
		globalViewport = dynamic_cast<MenuViewport*>(GetLevel()->GetGlobalViewport());
	}

	globalViewport->ShowMainMenu(false);
	globalViewport->ShowScoreWidget(true);
	globalViewport->GetScoreWidget()->SetScore(score);
	globalViewport->GetScoreWidget()->SetHighscore(highscore);
}

void BackdoomsGameState::SetPlayerBindings(BackdoomsPlayerHandle* playerHandle)
{
	Player* p = dynamic_cast<Player*>(playerHandle->GetPlayerObject());
	if (p == nullptr)
	{
		return;
	}
	if (p->GetHittableComponent() == nullptr)
	{
		return;
	}
	
	onPlayerReachedEnd += p->exitTouched.Subscribe(this, &BackdoomsGameState::OnEndReached);
	onPlayerDied += p->GetHittableComponent()->onKilled.Subscribe(this, &BackdoomsGameState::OnDied);
}

void BackdoomsGameState::SetupLevel()
{
	srand(time(0));

	if (GetLevel() == nullptr)
	{
		return;
	}
	GetLevel()->GenerateLevel();

	playersReachedEnd = 0;

	PositionPlayers();
	GenerateExit();
	GenerateWalls();
	GeneratePickups();
	GenerateMonsters();
}

void BackdoomsGameState::PositionPlayers()
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	size_t n = playerHandles.size();
	for (size_t i = 0; i < n; i++)
	{
		if (IsValid(playerHandles[i]->GetPlayerObject()) == false)
		{
			continue;
		}

		Vector2 spawnpoint = TileCoordinatesToRealCoordinates(GetRoomCoordinates(rand() % 25));
		int rotation = 90 * (rand() % 4);
		playerHandles[i]->GetPlayerObject()->SetTransform(Transform(spawnpoint, rotation));
		playerHandles[i]->GetPlayerObject()->SetActive(true);
		playerHandles[i]->OnRespawned();
	}
}

void BackdoomsGameState::GenerateExit()
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	int doorCoord1 = (rand() % 2) * 20;
	int doorCoord2 = (rand() % 4) * 4 + 2;

	if (rand() % 2 == 1)
	{
		GetLevel()->SetWallTile(doorCoord1, doorCoord2, 2);
		printf("Door coords: %d, %d - %d\n", doorCoord1, doorCoord2, GetLevel()->GetTile(doorCoord1, doorCoord2));
	}
	else
	{
		GetLevel()->SetWallTile(doorCoord2, doorCoord1, 2);
		printf("Door coords: %d, %d - %d\n", doorCoord2, doorCoord1, GetLevel()->GetTile(doorCoord2, doorCoord1));
	}
}

void BackdoomsGameState::GenerateWalls()
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	bool visited[ROOMS_X][ROOMS_Y] = {};
	bool opened[ROOMS_X][ROOMS_Y][2] = {};

	struct Cell { int x, y; };
	std::vector<Cell> stack;

	Cell start = { rand() % ROOMS_X, rand() % ROOMS_Y };
	visited[start.x][start.y] = true;
	stack.push_back(start);

	const int DX[4] = { 1, 0, -1, 0 };
	const int DY[4] = { 0, 1, 0, -1 };

	while (stack.empty() == false)
	{
		Cell current = stack.back();

		int options[4];
		int optionCount = 0;
		for (int d = 0; d < 4; ++d)
		{
			int nx = current.x + DX[d];
			int ny = current.y + DY[d];
			if (nx < 0 || ny < 0 || nx >= ROOMS_X || ny >= ROOMS_Y) { continue; }
			if (visited[nx][ny]) { continue; }
			options[optionCount++] = d;
		}

		if (optionCount == 0)
		{
			stack.pop_back();
			continue;
		}

		int d = options[rand() % optionCount];
		Cell next = { current.x + DX[d], current.y + DY[d] };

		int ex = std::min(current.x, next.x);
		int ey = std::min(current.y, next.y);
		int dir = (d == 0 || d == 2) ? 0 : 1;
		GenerateGap(ex, ey, dir, true);
		opened[ex][ey][dir] = true;

		visited[next.x][next.y] = true;
		stack.push_back(next);
	}

	for (int ry = 0; ry < ROOMS_Y; ++ry)
	{
		for (int rx = 0; rx < ROOMS_X; ++rx)
		{
			for (int dir = 0; dir < 2; ++dir)
			{
				if (dir == 0 && rx == ROOMS_X - 1) 
				{ 
					continue; 
				}
				if (dir == 1 && ry == ROOMS_Y - 1) 
				{ 
					continue; 
				}
				if (opened[rx][ry][dir]) 
				{
					continue; 
				}
				if (rand() % 100 < EXTRA_OPEN_PERCENT)
				{
					GenerateGap(rx, ry, dir, true);
				}
			}
		}
	}
}

void BackdoomsGameState::GenerateGap(int roomX, int roomY, int dir, bool open)
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	int x = dir == 0 ? (roomX + 1) * ROOM_STRIDE : roomX * ROOM_STRIDE + EXIT_OFFSET;
	int y = dir == 0 ? roomY * ROOM_STRIDE + EXIT_OFFSET : (roomY + 1) * ROOM_STRIDE;

	GetLevel()->SetWallTile(x, y, open ? 0 : 1);
}

void BackdoomsGameState::GeneratePickups()
{
	if (GetLevel() == nullptr)
	{
		return;
	}
	size_t n = pickups.size();
	for (size_t i = 0; i < n; i++)
	{
		if (IsValid(pickups[i]) == false)
		{
			continue;
		}
		pickups[i]->Destroy();
	}
	pickups.clear();

	Vector2 SpawnCoordinates;
	std::vector<Vector2> used;

	for (int i = 0; i < playerCount * 2; i++)
	{
		do
		{
			SpawnCoordinates = GetRoomCornerCoordinates(rand() % 25);
		} while (std::find(used.begin(), used.end(), SpawnCoordinates) != used.end());
		used.push_back(SpawnCoordinates);

		Soda* soda = GetLevel()->Spawn<Soda>(TileCoordinatesToRealCoordinates(SpawnCoordinates), 0.f, 10);
		pickups.push_back(soda);

		do
		{
			SpawnCoordinates = GetRoomCornerCoordinates(rand() % 25);
		} while (std::find(used.begin(), used.end(), SpawnCoordinates) != used.end());
		used.push_back(SpawnCoordinates);

		Ammo* ammo = GetLevel()->Spawn<Ammo>(TileCoordinatesToRealCoordinates(SpawnCoordinates), 0.f, 10);
		pickups.push_back(ammo);
	}
}

void BackdoomsGameState::GenerateMonsters()
{
	if (GetLevel() == nullptr)
	{
		return;
	}
	size_t n = monsters.size();
	for (size_t i = 0; i < n; i++)
	{
		if (IsValid(monsters[i]) == false)
		{
			continue;
		}
		monsters[i]->Destroy();
	}
	monsters.clear();

	Vector2 RoomCoordinates;
	std::vector<Vector2> used;

	for (int i = 0; i < playerCount; i++)
	{
		RoomCoordinates = GetRoomCoordinates(rand() % 25);
		Monster* monster = GetLevel()->Spawn<Monster>(TileCoordinatesToRealCoordinates(RoomCoordinates), 0.f);
		monsters.push_back(monster);
	}
}

void BackdoomsGameState::OnDied()
{
	alivePlayers--;
	if (alivePlayers > 0)
	{
		return;
	}

	isOver = true;
	playerCount == 2 ? save.SubmitMultiplayerScore(highscore) : save.SubmitSingleplayerScore(highscore);
}

void BackdoomsGameState::OnEndReached()
{
	playersReachedEnd++;

	if (playersReachedEnd < alivePlayers)
	{
		return;
	}

	score++;
	if (score > highscore) highscore = score;

	globalViewport->GetScoreWidget()->SetScore(score);
	globalViewport->GetScoreWidget()->SetHighscore(highscore);

	SetupLevel();
}

void BackdoomsGameState::EscPressed()
{
	if (isStarted == false)
	{
		Engine::Quit();
		return;
	}
	if (isPaused || isOver)
	{
		isPaused = false;
		isStarted = false;
		isOver = false;
		Engine::SetTimeScale(1.f);

		globalViewport->ShowPauseWidget(false);
		globalViewport->ShowScoreWidget(false);
		globalViewport->ShowMainMenu(true);

		return;
	}
	if (isPaused == false)
	{
		isPaused = true;
		Engine::SetTimeScale(0.f);

		globalViewport->ShowPauseWidget(true);

		return;
	}
}

void BackdoomsGameState::EnterPressed()
{
	if (isOver)
	{
		isPaused = false;
		isStarted = false;
		isOver = false;
		Engine::SetTimeScale(1.f);

		globalViewport->ShowPauseWidget(false);
		globalViewport->ShowScoreWidget(false);
		globalViewport->ShowMainMenu(true);
	}
	if (isPaused)
	{
		isPaused = false;
		Engine::SetTimeScale(1.f);

		globalViewport->ShowPauseWidget(false);
	}
}

bool BackdoomsGameState::IsValidRoomId(int id)
{
	return id >= 0 && id <= 24;
}

Vector2 BackdoomsGameState::TileCoordinatesToRealCoordinates(Vector2 in)
{
	return Vector2((in.x + 0.5) * TILE_SIZE, (in.y + 0.5) * TILE_SIZE);
}

Vector2 BackdoomsGameState::GetRoomCoordinates(int roomId)
{
	if (IsValidRoomId(roomId) == false)
	{
		return Vector2();
	}

	int x = roomId % 5;
	int y = roomId / 5;

	printf("roomId - %d, x - %d, y - %d\n", roomId, x, y);
	return Vector2(2 + x * 4, 2 + y * 4);
}

Vector2 BackdoomsGameState::GetRoomCornerCoordinates(int roomId)
{
	Vector2 RoomCoordinates = GetRoomCoordinates(roomId);
	return RoomCoordinates + Vector2((rand() % 2 == 0 ? -1 : 1) * TILE_SIZE, (rand() % 2 == 0 ? -1 : 1) * TILE_SIZE);
}
