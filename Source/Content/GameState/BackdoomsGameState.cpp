#include "BackdoomsGameState.h"

#include <windows.h>
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


void BackdoomsGameState::OnStart()
{
	
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
	if (Input::GetKeyState(VK_ESCAPE) == pressed)
	{
		EscPressed();
	}
	if (Input::GetKeyState(VK_RETURN) == pressed)
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

	playerHandles.clear();

	isStarted = true;
	this->playerCount = playerCount;

	if (playerCount == 2)
	{
		playerHandles.push_back(GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::TopHalf(), ControlScheme::PlayerOne()));
		playerHandles.push_back(GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::BottomHalf(), ControlScheme::PlayerTwo()));
	}
	else
	{
		playerHandles.push_back(GetLevel()->Spawn<BackdoomsPlayerHandle>(Vector2(), 0.f, Window::Fullscreen(), ControlScheme::PlayerOne()));
	}

	SetupLevel();
}

void BackdoomsGameState::SetupLevel()
{
	srand(time(0));

	if (GetLevel() == nullptr)
	{
		return;
	}

	pickups.clear();
	monsters.clear();

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

	for (BackdoomsPlayerHandle* ph : playerHandles)
	{
		if (ph->GetPlayerObject() == nullptr)
		{
			continue;
		}
		Vector2 spawnpoint = TileCoordinatesToRealCoordinates(GetRoomCoordinates(rand() % 25));
		ph->GetPlayerObject()->SetPosition(spawnpoint);
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

}

void BackdoomsGameState::GeneratePickups()
{
	if (GetLevel() == nullptr)
	{
		return;
	}
	for (Pickup* p : pickups)
	{
		p->Destroy();
	}

	Vector2 SpawnCoordinates;
	vector<Vector2> used;

	srand(time(0));
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
	for (Monster* m : monsters)
	{
		m->Destroy();
	}

	Vector2 RoomCoordinates;
	vector<Vector2> used;

	srand(time(0));
	for (int i = 0; i < playerCount; i++)
	{
		RoomCoordinates = GetRoomCoordinates(rand() % 25);
		Monster* monster = GetLevel()->Spawn<Monster>(TileCoordinatesToRealCoordinates(RoomCoordinates), 0.f);
		monsters.push_back(monster);
	}
}

void BackdoomsGameState::EscPressed()
{
	if (isPaused == false)
	{
		isPaused = true;
		Engine::SetTimeScale(0.f);
		printf("Paused, press again to exit\n");
		return;
	}
	if (isPaused)
	{
		Engine::SetTimeScale(1.f);
		Engine::Quit();
	}
}

void BackdoomsGameState::EnterPressed()
{
	if (isPaused)
	{
		printf("UnPaused\n");
		isPaused = false;
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
