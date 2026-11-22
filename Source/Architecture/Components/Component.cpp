#include "Component.h"

Object* Component::GetOwner()
{
	return owner;
}

bool Component::IsActive()
{
	return isActive;
}

void Component::SetActive(bool value)
{
	isActive = value;
}
