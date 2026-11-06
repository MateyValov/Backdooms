#pragma once

#include "../Structs/Transform.h"

#include "Components/Component.h"

#include <algorithm>
#include <cstdio>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>


class Object
{
private:
	Object* parent = nullptr;
	Transform localTransform = Transform();

	bool begun = false;

	void SweepRemoved();

protected:
	Transform transform;

	std::vector<std::unique_ptr<Component>> components;

	virtual void OnStart() {}
	virtual void OnUpdate() {}

public:
	Object() = default;
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;
	virtual ~Object() = default;

	void Start();
	void Update();
	void Destroy();

	template <typename T, typename... Args>
	T* AddComponent(Args&&... args);

	template <typename T>
	T* GetComponent();

	template <typename T>
	std::vector<T*> GetAllComponentOfClass();

	void RemoveComponent(Component* component);

	Transform GetTransform() const;
	Transform GetLocalTransform() const;

	void SetParent(Object* parent, Transform offset = Transform());
	void RemoveParent();
	Object* GetParent();
	Object* GetGrandParent();

	void SetTransform(Transform newTrasform);
	void SetPosition(Vector2 newPosition);
	void SetAngle(float newAngle);

	virtual void OnCollisionEnter(Object* other) {}
	virtual void OnCollisionStay(Object* other) {}
	virtual void OnCollisionExit(Object* other) {}

	virtual void OnWallEnter(int tileX, int tileY, int wallValue) {}
	virtual void OnWallStay(int tileX, int tileY, int wallValue) {}
	virtual void OnWallExit(int tileX, int tileY, int wallValue) {}
};

template <typename T, typename... Args>
T* Object::AddComponent(Args&&... args)
{
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
	std::unique_ptr<T> owned = std::make_unique<T>(std::forward<Args>(args)...);
	T* component = owned.get();
	components.push_back(std::move(owned));
	component->owner = this;
	if (begun) { component->Start(); }
	return component;
}

template <typename T>
T* Object::GetComponent()
{
	for (auto& c : components) 
	{ 
		if (c->removed) 
		{ 
			continue;
		} 
		if (T* t = dynamic_cast<T*>(c.get()))
		{
			return t;
		}
	}
	return nullptr;
}

template <typename T>
std::vector<T*> Object::GetAllComponentOfClass()
{
	std::vector<T*> output;
	for (auto& c : components)
	{
		if (c->removed)
		{
			continue;
		}
		if (T* t = dynamic_cast<T*>(c.get()))
		{
			output.push_back(t);
		}
	}
	return output;
}