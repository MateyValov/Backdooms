#include "Player.h"

#include "../Structs/MathFuncs.h"
#include "../Engine/Engine.h"
#include "Map.h"

Player::Player() : MovableObject()
{
	health = PLAYER_MAX_HEALTH;
	ammo = PLAYER_MAX_AMMO;
}

Player::Player(float radius, int movementSpeed, int turnSpeed) : MovableObject(radius, movementSpeed, turnSpeed)
{
	health = PLAYER_MAX_HEALTH;
	ammo = PLAYER_MAX_AMMO;
}

void Player::Start()
{
}

void Player::Update()
{
}

int Player::GetHealth()
{
	return health;
}

int Player::GetAmmo()
{
	return ammo;
}

void Player::AddHealth(int Amount)
{
	health = clamp(health + Amount, 0, PLAYER_MAX_HEALTH);
}

void Player::AddAmmo(int Amount)
{
	ammo = clamp(ammo + Amount, 0, PLAYER_MAX_AMMO);
}

void Player::InteractPressed()
{
	Vector2 TargetPoint = transform.position + transform.GetForward() * (radius * 1.5f);

	int tileX = floor(TargetPoint.x / TILE_SIZE);
	int tileY = floor(TargetPoint.y / TILE_SIZE);

	printf("Facing tile with index #%d, looking at %.f, %.f\n", 
		Engine::GetMap()->GetTile(tileX, tileY),
		TargetPoint.x,
		TargetPoint.y);
}

void Player::ShootPressed()
{
}
