#include "Player.h"

#include "Architecture/Components/Collider.h"
#include "Architecture/Components/Sprite.h"
#include "Architecture/Level.h"
#include "Structs/MathFuncs.h"
#include "Modules/Engine.h"
#include "Modules/Physics.h"

#include "Content/Components/HittableComponent.h"
#include "Pickup.h"


Player::Player() : MovableObject()
{
	this->maxAmmo = 8;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(8);
	AddComponent<Sprite>(16, 17, 18, 19);
	AddComponent<Collider>(20);
}

Player::Player(int health, int ammo) : MovableObject()
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(health);
	AddComponent<Sprite>(16, 17, 18, 19);
	AddComponent<Collider>(20);
}

Player::Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed) : MovableObject(movementSpeed, turnSpeed)
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(8);
	AddComponent<Sprite>(16, 17, 18, 19);
	AddComponent<Collider>(radius);
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
		exitTouched.Invoke();
	}
}

HittableComponent* Player::GetHittableComponent()
{
	return hittableComp;
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
	if (GetLevel() == nullptr || ammo <= 0)
	{
		return;
	}
	SetAmmo(ammo - 1);

	RaycastHit hit = Physics::Raycast(GetLevel(), transform.position, transform.position + transform.GetForward() * 640, this);
	if (hit.object == nullptr)
	{
		return;
	}
	HittableComponent* otherHittableComp = hit.object->GetComponent<HittableComponent>();
	if (otherHittableComp == nullptr)
	{
		return;
	}
	otherHittableComp->TakeDamage(1);
}
