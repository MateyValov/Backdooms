#include "BackdoomsGameState.h"

#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include "Architecture/Level.h"
#include "Modules/Input.h"
#include "Modules/Engine.h"

#include "Content/Objects/Soda.h"
#include "Content/Objects/Ammo.h"
#include "Content/Objects/Monster.h"
#include "Content/Objects/Player.h"


BackdoomsGameState::BackdoomsGameState()
{

}

void BackdoomsGameState::OnStart()
{
	SetupLevel();
}

void BackdoomsGameState::OnUpdate()
{
	if (Input::GetKeyState(VK_ESCAPE) == pressed)
	{
		EscPressed();
	}
	if (Input::GetKeyState(VK_RETURN) == pressed)
	{
		EscPressed();
	}
}

void BackdoomsGameState::SetupLevel()
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	playerCount = GetLevel()->GetObjectsOfClass<Player>().size();

	GenerateExit();
	GenerateWalls();
	GeneratePickups(2);
	GenerateMonsters();
}

void BackdoomsGameState::GenerateExit()
{
	if (GetLevel() == nullptr)
	{
		return;
	}

	srand(time(0));

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

void BackdoomsGameState::GeneratePickups(int perTypeAmount)
{
	if (GetLevel() == nullptr)
	{
		return;
	}
	for (Pickup* p : pickups)
	{
		p->Destroy();
	}

	Vector2 RoomCoordinates;
	vector<Vector2> used;

	srand(time(0));
	for (int i = 0; i < perTypeAmount; i++)
	{
		RoomCoordinates = GetRoomCoordinates(rand() % 25);
		Soda* soda = GetLevel()->Spawn<Soda>(TileCoordinatesToRealCoordinates(RoomCoordinates), 0.f, 10);
		pickups.push_back(soda);

		RoomCoordinates = GetRoomCoordinates(rand() % 25);
		Ammo* ammo = GetLevel()->Spawn<Ammo>(TileCoordinatesToRealCoordinates(RoomCoordinates), 0.f, 10);
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