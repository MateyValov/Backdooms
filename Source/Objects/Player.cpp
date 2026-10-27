#include "Player.h"

#include "../Structs/MathFuncs.h"

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
