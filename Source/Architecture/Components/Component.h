#pragma once

class Object;

class Component
{
private:
	friend class Object;
	bool removed = false;

protected:
	Object* owner = nullptr;

	virtual void Start() {}
	virtual void Update() {}
	virtual void OnDestroy() {}

public:
	virtual ~Component() = default;

	Object* GetOwner();
};
