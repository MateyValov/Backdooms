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


void BackdoomsGameState::OnStart()
{
	globalViewport = dynamic_cast<MenuViewport*>(GetLevel()->GetGlobalViewport());
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
	for (PlayerHandle* ph : playerHandles)
	{
		ph->Destroy();
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

	if (globalViewport == nullptr)
	{
		globalViewport = dynamic_cast<MenuViewport*>(GetLevel()->GetGlobalViewport());
	}
	globalViewport->ShowMainMenu(false);
	globalViewport->ShowScoreWidget(true);
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

	for (BackdoomsPlayerHandle* ph : playerHandles)
	{
		if (ph->GetPlayerObject() == nullptr)
		{
			continue;
		}
		Vector2 spawnpoint = TileCoordinatesToRealCoordinates(GetRoomCoordinates(rand() % 25));
		int rotation = 90 * (rand() % 4);
		ph->GetPlayerObject()->SetTransform(Transform(spawnpoint, rotation));
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
	for (Monster* m : monsters)
	{
		m->Destroy();
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
	if (alivePlayers <= 0)
	{
		isOver = true;
		printf("Game over!\n");
	}
}

void BackdoomsGameState::OnEndReached()
{
	playersReachedEnd++;
	printf("A player reached the end!\n");

	if (playersReachedEnd < alivePlayers)
	{
		printf("Waiting for all players (%d/%d)\n", playersReachedEnd, alivePlayers);
		return;
	}

	score++;
	if (score > highscore) highscore = score;
	printf("Level cleared! Score: %d, High Score: %d\n", score, highscore);
	SetupLevel();
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
