#pragma once

#include "Component.h"

#include "../../Structs/Transform.h"

class Sprite : public Component
{
protected:
	int textureFront;
	int textureLeft;
	int textureBack;
	int textureRight;

public:
	Sprite(int texture);
	Sprite(int textureFront, int textureLeft, int textureBack, int textureRight);

	void SetTextureId(int front, int left = -1, int back = -1, int right = -1);
	int GetTextureId() const;
	int GetTextureIdForPosition(Vector2 viewerPosition) const;
};
