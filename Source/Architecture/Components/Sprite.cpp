#include "Sprite.h"

#include "../Object.h"


Sprite::Sprite(int texture)
{
	this->textureFront = texture;
	this->textureLeft = texture;
	this->textureBack = texture;
	this->textureRight = texture;
}

Sprite::Sprite(int textureFront, int textureLeft, int textureBack, int textureRight)
{
	this->textureFront = textureFront;
	this->textureLeft = textureLeft;
	this->textureBack = textureBack;
	this->textureRight = textureRight;
}

void Sprite::SetTextureId(int front, int left, int back, int right)
{
	textureFront = front;
	textureLeft = left > -1 ? left : front;
	textureBack = back > -1 ? back : front;
	textureRight = right > -1 ? right : front;
}

int Sprite::GetTextureId() const
{
	return textureFront;
}

int Sprite::GetTextureIdForPosition(Vector2 viewerPosition) const
{
	if (owner == nullptr)
	{
		return textureFront;
	}

	float toViewer = atan2f(-(viewerPosition.y - owner->GetTransform().position.y), viewerPosition.x - owner->GetTransform().position.x) * 180.0f / PI;
	float relative = clampAngle(fmodf(toViewer - owner->GetTransform().angle + 180.0f, 360.0f));
	relative -= 180.0f;

	int texture = textureFront;
	if (relative >= 45.0f && relative < 135.0f)
	{
		texture = textureLeft;
	}
	else if (relative >= -135.0f && relative < -45.0f)
	{
		texture = textureRight;
	}
	else if (relative >= 135.0f || relative < -135.0f)
	{
		texture = textureBack;
	}

	return texture >= 0 ? texture : textureFront;
}
