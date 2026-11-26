#pragma once

#include "Architecture/Components/Component.h"

class AnimatedSprite;

class AnimationHandlerComponent : public Component
{
private:
	AnimatedSprite* reference;

protected:
	void Start() override;
	void Update() override;

public:
	AnimationHandlerComponent() = default;

};