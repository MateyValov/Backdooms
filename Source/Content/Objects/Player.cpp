#include "Player.h"

#include "Architecture/Components/AnimatedSprite.h"
#include "Architecture/Components/Collider.h"
#include "Architecture/Level.h"
#include "Structs/MathFuncs.h"
#include "Modules/Engine.h"
#include "Modules/Physics.h"
#include "Modules/Audio.h"

#include "Content/Components/AnimationHandlerComponent.h"
#include "Content/Components/HittableComponent.h"
#include "Pickup.h"


void Player::InitRenderer()
{
	AnimatedSprite* renderer = AddComponent<AnimatedSprite>(Animation(std::vector<Frame>
	{
		Frame(16, 17, 18, 19)
	}));
	renderer->AddAnimation(Animation(std::vector<Frame>
	{
		Frame(24, 25, 26, 27),
			Frame(32, 33, 34, 35),
			Frame(40, 41, 42, 43),
			Frame(48, 49, 50, 51),
	}), "walk");
}

Player::Player() : MovableObject()
{
	this->maxAmmo = 8;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(8);
	InitRenderer();
	AddComponent<Collider>(20);
	AddComponent<AnimationHandlerComponent>();
}

Player::Player(int health, int ammo) : MovableObject()
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(health);
	InitRenderer();
	AddComponent<Collider>(20);
	AddComponent<AnimationHandlerComponent>();
}

Player::Player(int health, int ammo, float radius, int movementSpeed, int turnSpeed) : MovableObject(movementSpeed, turnSpeed)
{
	this->maxAmmo = ammo;
	this->ammo = maxAmmo;

	hittableComp = AddComponent<HittableComponent>(health);
	InitRenderer();
	AddComponent<Collider>(20);
	AddComponent<AnimationHandlerComponent>();
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
		SetActive(false);
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
	Audio::PlaySoundEffect("gunshot.wav");

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
