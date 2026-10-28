#pragma once

#include "../Structs/Transform.h"

class Object
{
protected:
	Transform transform;
	float radius;
	bool hasCollision;

public:
	Object();
	Object(float radius);

	virtual void Start() = 0;
	virtual void Update() = 0;

	Transform GetTransform() const;
	float GetRadius() const;
	bool HasCollision() const;

	void SetTransform(Transform newTrasform);
	void SetPosition(Vector2 newPosition);
	void SetAngle(float newAngle);
};