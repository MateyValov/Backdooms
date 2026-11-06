#pragma once

#include "Component.h"

class Collider : public Component
{
private:
	bool isActive = true;
	float radius;
	
public:
	Collider(float radius);

	bool IsActive();
	void SetActive(bool value);

	float GetRadius() const;
};