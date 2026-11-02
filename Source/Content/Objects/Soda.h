#pragma once

#include "Pickup.h"

class Soda : public Pickup
{
public:
	Soda();
	Soda(float radius, int value);

	void Obtain(Player* player) override;
};
