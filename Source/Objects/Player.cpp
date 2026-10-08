#include "Player.h"

float PLAYER_X;
float PLAYER_Y;
float PLAYER_DELTA_X = 0.f;
float PLAYER_DELTA_Y = 0.f;
float PLAYER_ANGLE = 0.f;

Player::Player()
{
	health = PLAYER_MAX_HEALTH;
	ammo = PLAYER_MAX_AMMO;
}

int Player::GetHealth()
{
	return health;
}

int Player::GetAmmo()
{
	return ammo;
}
