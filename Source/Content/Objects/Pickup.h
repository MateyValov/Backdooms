#pragma once

#include "../../Architecture/Object.h"

class Player;

class Pickup : public Object
{
protected:
	int value = 1;

public:
	Pickup() : Object() {};
	Pickup(float radius, int value);

	void SetValue(int value);
	int GetValue() const;

	virtual void Obtain(Player* player) = 0;
};