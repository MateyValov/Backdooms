#pragma once

#include "Component.h"

class Collider : public Component
{
private:
	float radius;
	
public:
	Collider(float radius);

	float GetRadius() const;
};