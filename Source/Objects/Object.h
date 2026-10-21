#pragma once

#include "../Utility/UtilityStructs.h"

class Object
{
protected:
	Transform transform;
	float radius;

public:
	Object();
	Object(float radius);

	virtual void Start() = 0;
	virtual void Update() = 0;

	Transform GetTransform() const;
	float GetRadius() const;

	void SetTransform(Transform newTrasform);
	void SetPosition(Vector2 newPosition);
	void SetAngle(float newAngle);
};