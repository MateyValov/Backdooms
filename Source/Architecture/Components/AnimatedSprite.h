#pragma once

#include "Renderer.h"

#include <map>
#include <vector>
#include <string>

struct Frame
{
	int textureFront;
	int textureLeft;
	int textureBack;
	int textureRight;

	Frame(int textureFront) : textureFront(textureFront), textureLeft(textureFront), textureBack(textureFront), textureRight(textureFront) {}
	Frame(int textureFront, int textureLeft, int textureBack, int textureRight) : textureFront(textureFront), textureLeft(textureLeft), textureBack(textureBack), textureRight(textureRight) {}
};

struct Animation
{
	std::vector<Frame> frames = {Frame(0)};
	float frameLength = 0.15f;

	Animation() = default;
	Animation(std::vector<Frame> frames) : frames(frames) {}
	Animation(std::vector<Frame> frames, float frameLength) : frames(frames), frameLength(frameLength) {}

	float Duration() const
	{
		return frames.size() * frameLength;
	}

	Frame GetFrame(float time) const
	{
		float duration = Duration();
		while (time > duration)
		{
			time -= duration;
		}
		int frameId = 0;
		while (time > frameLength)
		{
			time -= frameLength;
			frameId++;
		}
		return frames[frameId];
	}
};

class AnimatedSprite : public Renderer
{
private:
	std::map<std::string, Animation> animations;
	std::string currentAnimationId = "";

	float animationTime;
	float maxAnimationTime;

	bool isPaused = false;
	bool stopFlag = false;
	bool reverseFlag = false;

protected:
	void Update() override;

public:
	AnimatedSprite(Animation baseAnimation);
	AnimatedSprite(Animation baseAnimation, Color color);

	int GetTextureId() const override;
	int GetTextureIdForPosition(Vector2 viewerPosition) const override;

	void AddAnimation(Animation animation, std::string animationId);

	void PlayAnimation(std::string animationId, bool isReversed = false);
	void StopAnimation(std::string animationId, bool waitForCompletion = false);

	void SetPause(bool value);
};
