#include "AnimationHandlerComponent.h"

#include "Architecture/Object.h"
#include "Architecture/Components/AnimatedSprite.h"

void AnimationHandlerComponent::Start()
{
	reference = GetOwner()->GetComponent<AnimatedSprite>();
}

void AnimationHandlerComponent::Update()
{
	if (reference == nullptr)
	{
		return;
	}
	if (GetOwner()->GetSpeed() > 0.1f)
	{
		Vector2 velocity = GetOwner()->GetVelocity();
		velocity.normalize();
		float a = Vector2::angle(velocity, GetOwner()->GetTransform().GetForward());
		reference->PlayAnimation("walk", a > 90.f);
	}
	else
	{
		reference->PlayAnimation("base");
	}
	
}
