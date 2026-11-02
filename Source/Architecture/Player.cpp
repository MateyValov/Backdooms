#include "Player.h"

#include "Map.h"
#include "../Structs/MathFuncs.h"
#include "../Modules/Engine.h"


Player::Player() : MovableObject()
{
	maxHealth = 8;
	maxAmmo = 8;

	health = maxHealth;
	ammo = maxAmmo;

	SetTextureId(16,17,18,19);
}

Player::Player(int health, int ammo) : MovableObject()
{
	this->maxHealth = health;
	this->maxAmmo = ammo;

	this->health = maxHealth;
	this->ammo = maxAmmo;

	SetTextureId(16, 17, 18, 19);
}

Player::Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed) : MovableObject(radius, movementSpeed, turnSpeed)
{
	this->maxHealth = health;
	this->maxAmmo = ammo;

	this->health = maxHealth;
	this->ammo = maxAmmo;

	SetTextureId(16, 17, 18, 19);
}

void Player::OnCollisionEnter(Object* other)
{
	printf("COLLIDE");
}

void Player::OnWallEnter(int tileX, int tileY, int wallValue)
{
	if (wallValue == 2)
	{
		printf("Time to exit level\n");
	}
}

int Player::GetHealth() const
{
	return health;
}

int Player::GetMaxHealth() const
{
	return maxHealth;
}

int Player::GetAmmo() const
{
	return ammo;
}

int Player::GetMaxAmmo() const
{
	return maxAmmo;
}

void Player::AddHealth(int Amount)
{
	health = clamp(health + Amount, 0, maxHealth);
}

void Player::AddAmmo(int Amount)
{
	ammo = clamp(ammo + Amount, 0, maxAmmo);
}

void Player::InteractPressed()
{	
}
