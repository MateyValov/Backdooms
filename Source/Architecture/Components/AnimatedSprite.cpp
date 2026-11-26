#include "AnimatedSprite.h"

#include "Architecture/Object.h"
#include "Modules/Engine.h"


AnimatedSprite::AnimatedSprite(Animation baseAnimation)
{
	AddAnimation(baseAnimation, "base");
	PlayAnimation("base");
}

AnimatedSprite::AnimatedSprite(Animation baseAnimation, Color color) : Renderer(color)
{
	AddAnimation(baseAnimation, "base");
	PlayAnimation("base");
}

void AnimatedSprite::Update()
{
	if (currentAnimationId.empty() || isPaused)
	{
		return;
	}

	if (reverseFlag)
	{
		animationTime -= Engine::GetDeltaTime();
		if (animationTime >= 0.f)
		{
			return;
		}
		animationTime = maxAnimationTime;
	}
	else
	{
		animationTime += Engine::GetDeltaTime();
		if (animationTime <= maxAnimationTime)
		{
			return;
		}
		animationTime = 0.f;
	}
	
	if (stopFlag)
	{
		currentAnimationId = "base";
		stopFlag = false;
	}
}

int AnimatedSprite::GetTextureId() const
{
	Frame currentFrame = animations.at(currentAnimationId).GetFrame(animationTime);
	return currentFrame.textureFront;
}

int AnimatedSprite::GetTextureIdForPosition(Vector2 viewerPosition) const
{
	Frame currentFrame = animations.at(currentAnimationId).GetFrame(animationTime);
	if (owner == nullptr)
	{
		return currentFrame.textureFront;
	}

	float relative = Transform::angleToSigned(owner->GetTransform(), viewerPosition);
	float magnitude = fabsf(relative);

	int texture = currentFrame.textureFront;
	if (magnitude >= 135.0f)
	{
		texture = currentFrame.textureBack;
	}
	else if (magnitude >= 45.0f)
	{
		texture = relative > 0.0f ? currentFrame.textureLeft : currentFrame.textureRight;
	}

	return texture;
}

void AnimatedSprite::AddAnimation(Animation animation, std::string animationId)
{
	animations[animationId] = animation;
}

void AnimatedSprite::PlayAnimation(std::string animationId, bool isReversed)
{
	if (currentAnimationId == animationId)
	{
		return;
	}

	auto it = animations.find(animationId);
	if (it == animations.end())
	{
		return;
	}

	maxAnimationTime = animations[animationId].Duration();
	animationTime = isReversed ? maxAnimationTime : 0.f;
	reverseFlag = isReversed;
	currentAnimationId = animationId;
}

void AnimatedSprite::StopAnimation(std::string animationId, bool waitForCompletion)
{
	auto it = animations.find(animationId);
	if (it == animations.end())
	{
		return;
	}

	if (waitForCompletion)
	{
		stopFlag = true;
	}
	else
	{
		currentAnimationId = "base";
	}
}

void AnimatedSprite::SetPause(bool value)
{
	isPaused = value;
}
