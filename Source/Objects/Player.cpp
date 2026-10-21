#include "Player.h"
#include "../Engine/Engine.h"

#include <math.h>

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
