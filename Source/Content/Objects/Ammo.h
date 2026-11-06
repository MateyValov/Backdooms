#pragma once

#include "Pickup.h"

class Ammo : public Pickup
{
public:
	Ammo();
	Ammo(int value);

	void Obtain(Player* player) override;
};