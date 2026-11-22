#pragma once

class Object;

class Component
{
private:
	friend class Object;
	bool removed = false;
	bool isActive = true;

protected:
	Object* owner = nullptr;

	virtual void Start() {}
	virtual void Update() {}
	virtual void OnDestroy() {}

public:
	virtual ~Component() = default;

	Object* GetOwner();

	bool IsActive();
	void SetActive(bool value);
};
