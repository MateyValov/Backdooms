#pragma once

#include "Renderer.h"

class Sprite : public Renderer
{
protected:
	int textureFront;
	int textureLeft;
	int textureBack;
	int textureRight;

public:
	Sprite(int texture);
	Sprite(int texture, Color color);
	Sprite(int textureFront, int textureLeft, int textureBack, int textureRight);
	Sprite(int textureFront, int textureLeft, int textureBack, int textureRight, Color color);

	void SetTextureId(int front, int left = -1, int back = -1, int right = -1);
	virtual int GetTextureId() const override;
	virtual int GetTextureIdForPosition(Vector2 viewerPosition) const override;
};
