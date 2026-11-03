#include "Object.h"

#include "Map.h";
#include "../Structs/MathFuncs.h"
#include "../Modules/Engine.h"


Object::Object()
{
	radius = 20.f;
	hasCollision = true;
}

Object::Object(float radius)
{
	this->radius = radius;
	hasCollision = true;
}

void Object::Destroy()
{
	markedForDestruction = true;
	Engine::GetMap()->Destroy(this);
}

bool Object::MarkedForDestruction()
{
	return markedForDestruction;
}

Transform Object::GetTransform() const
{
	return transform;
}

float Object::GetRadius() const
{
	return radius;
}

bool Object::HasCollision() const
{
	return hasCollision;
}

void Object::SetTransform(Transform newTrasform)
{
	SetPosition(newTrasform.position);
	SetAngle(newTrasform.angle);
}

void Object::SetPosition(Vector2 newPosition)
{
	transform.position = hasCollision ? Engine::GetMap()->EvaluatePosition(newPosition, radius) : newPosition;
}

void Object::SetAngle(float newAngle)
{
	transform.angle = clampAngle(newAngle);
}

void Object::SetTextureId(int front, int left, int back, int right)
{
	textureFront = front;
	textureLeft  = left >  -1  ? left  : front;
	textureBack = back > -1 ? back : front;
	textureRight = right > -1  ? right : front;
}

int Object::GetTextureId() const
{
	return textureFront;
}

int Object::GetTextureIdForPosition(Vector2 viewerPosition) const
{
	float toViewer = atan2f(-(viewerPosition.y - transform.position.y), viewerPosition.x - transform.position.x) * 180.0f / PI;

	float relative = clampAngle(fmodf(toViewer - transform.angle + 180.0f, 360.0f));
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
