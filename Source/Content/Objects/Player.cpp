#include "Player.h"

#include "../../Architecture/Map.h"
#include "../../Structs/MathFuncs.h"
#include "../../Modules/Engine.h"

#include "../Input/PlayerController.h"
#include "Pickup.h"


Player::Player() : MovableObject(), Hittable(8)
{
	this->maxAmmo = 8;
	this->ammo = maxAmmo;

	SetTextureId(16,17,18,19);
}

Player::Player(int health, int ammo) : MovableObject(), Hittable(health)
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	SetTextureId(16, 17, 18, 19);
}

Player::Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed) : MovableObject(radius, movementSpeed, turnSpeed), Hittable(health)
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	SetTextureId(16, 17, 18, 19);
}

void Player::Update()
{
	if (controller == nullptr)
	{
		return;
	}

	controller->UpdateController();

	if (controller->GetMovementInput().x != 0.f)
	{
		Turn(controller->GetMovementInput().x > 0);
	}
	
	Move(transform.GetForward() * controller->GetMovementInput().y);
}

void Player::OnCollisionEnter(Object* other)
{
	Pickup* pickup = dynamic_cast<Pickup*>(other);
	if (pickup != nullptr)
	{
		pickup->Obtain(this);
	}
}

void Player::OnWallEnter(int tileX, int tileY, int wallValue)
{
	if (wallValue == 2)
	{
		printf("Time to exit level\n");
	}
}

void Player::AssignController(PlayerController* controller)
{
	this->controller = controller;

	onInteractPressed.Unsubscribe();
	onInteractPressed = controller->interactPressed.Subscribe(this, &Player::InteractPressed);
}

int Player::GetAmmo() const
{
	return ammo;
}

int Player::GetMaxAmmo() const
{
	return maxAmmo;
}

void Player::SetAmmo(int value)
{
	ammo = clamp(value, 0, maxAmmo);
	ammoUpdated.Invoke(ammo, maxAmmo);
}

void Player::SetMaxAmmo(int value, bool restore)
{
	maxAmmo = value;
	if (restore)
	{
		ammo = maxAmmo;
	}
	ammoUpdated.Invoke(ammo, maxAmmo);
}

bool Player::AddAmmo(unsigned int amount)
{
	int before = ammo;
	SetAmmo(ammo + amount);
	return before != ammo;
}

void Player::InteractPressed()
{	
	SetAmmo(ammo - 1);
}
