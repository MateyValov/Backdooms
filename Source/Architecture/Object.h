#pragma once

#include "../Structs/Transform.h"

class Object
{
private:
	bool markedForDestruction = false;

protected:
	Transform transform;
	float radius;
	bool hasCollision;

	int textureFront = -1;
	int textureLeft = -1;
	int textureRight = -1;
	int textureBack = -1;

public:
	Object();
	Object(float radius);
	virtual ~Object() = default;

	virtual void Start() {}
	virtual void Update() {}

	virtual void OnCollisionEnter(Object* other) {}   
	virtual void OnCollisionStay(Object* other) {}
	virtual void OnCollisionExit(Object* other) {}  

	virtual void OnWallEnter(int tileX, int tileY, int wallValue) {}
	virtual void OnWallStay(int tileX, int tileY, int wallValue) {}
	virtual void OnWallExit(int tileX, int tileY, int wallValue) {}

	void Destroy();
	bool MarkedForDestruction();

	Transform GetTransform() const;
	float GetRadius() const;
	bool HasCollision() const;

	void SetTransform(Transform newTrasform);
	void SetPosition(Vector2 newPosition);
	void SetAngle(float newAngle);

	void SetTextureId(int front, int left = -1, int right = -1, int back = -1);
	int GetTextureId() const;
	int GetTextureIdForPosition(Vector2 viewerPosition) const; 
};