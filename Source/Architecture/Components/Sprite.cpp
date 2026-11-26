#include "Sprite.h"

#include "../Object.h"


Sprite::Sprite(int texture)
{
	this->textureFront = texture;
	this->textureLeft = texture;
	this->textureBack = texture;
	this->textureRight = texture;
}

Sprite::Sprite(int texture, Color color) : Sprite(texture)
{
	this->color = color;
}

Sprite::Sprite(int textureFront, int textureLeft, int textureBack, int textureRight)
{
	this->textureFront = textureFront;
	this->textureLeft = textureLeft;
	this->textureBack = textureBack;
	this->textureRight = textureRight;
}

Sprite::Sprite(int textureFront, int textureLeft, int textureBack, int textureRight, Color color) : Sprite(textureFront, textureLeft, textureBack, textureRight)
{
	this->color = color;
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

	float relative = Transform::angleToSigned(owner->GetTransform(), viewerPosition);
	float magnitude = fabsf(relative);

	int texture = textureFront;
	if (magnitude >= 135.0f)
	{
		texture = textureBack;
	}
	else if (magnitude >= 45.0f)
	{
		texture = relative > 0.0f ? textureLeft : textureRight;
	}

	return texture >= 0 ? texture : textureFront;
}
