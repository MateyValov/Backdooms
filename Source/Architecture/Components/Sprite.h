#pragma once

#include "Component.h"

#include "Structs/Transform.h"
#include "Structs/Image.h"

class Sprite : public Component
{
protected:
	int textureFront;
	int textureLeft;
	int textureBack;
	int textureRight;
	Color color = Color();

public:
	Sprite(int texture);
	Sprite(int texture, Color color);
	Sprite(int textureFront, int textureLeft, int textureBack, int textureRight);
	Sprite(int textureFront, int textureLeft, int textureBack, int textureRight, Color color);

	void SetTextureId(int front, int left = -1, int back = -1, int right = -1);
	int GetTextureId() const;
	int GetTextureIdForPosition(Vector2 viewerPosition) const;
	
	void SetColor(Color color);
	Color GetColor() const;
};
