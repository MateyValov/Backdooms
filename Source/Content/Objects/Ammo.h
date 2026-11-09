#pragma once

#include "Pickup.h"

class Ammo : public Pickup
{
public:
	Ammo(float radius);
	Ammo(float radius, int value);

	void Obtain(Player* player) override;
};