#include "Object.h"

#include "Level.h";
#include "../Structs/MathFuncs.h"
#include "../Modules/Engine.h"


void Object::SweepRemoved()
{
	for (auto& c : components) 
	{ 
		if (c->removed) 
		{
			c->OnDestroy(); 
		} 
	}
	components.erase(std::remove_if(components.begin(), components.end(), 
		[](const std::unique_ptr<Component>& c) { return c->removed; }), components.end());
}

void Object::Start()
{
	size_t n = components.size();
	for (size_t i = 0; i < n; ++i) 
	{ 
		components[i]->Start(); 
	} 
	OnStart();
}

void Object::Update()
{
	if (parent != nullptr)
	{
		transform = parent->GetTransform() + localTransform;
	}

	size_t count = components.size();           
	for (size_t i = 0; i < count; ++i) 
	{ 
		if (!components[i]->removed) 
		{ 
			components[i]->Update(); 
		} 
	}
	OnUpdate();                                    
	SweepRemoved();
}

void Object::Destroy()
{
	Engine::GetLevel()->Destroy(this);
}

void Object::RemoveComponent(Component* component)
{
	for (auto& c : components) 
	{ 
		if (c.get() == component) 
		{
			c->removed = true; 
		} 
	}
}

Transform Object::GetTransform() const
{
	return transform;
}

Transform Object::GetLocalTransform() const
{
	return parent != nullptr ? localTransform : transform;
}

void Object::SetParent(Object* parent, Transform offset)
{
	if (parent == nullptr)
	{
		return;
	}
	this->parent = parent;
	localTransform = offset;
}

void Object::RemoveParent()
{
	parent = nullptr;
	localTransform = Transform();
}

Object* Object::GetParent()
{
	return parent;
}

Object* Object::GetGrandParent()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	Object* result = parent->GetGrandParent();
	return result != this ? result : nullptr;
}

void Object::SetTransform(Transform newTrasform)
{
	SetPosition(newTrasform.position);
	SetAngle(newTrasform.angle);
}

void Object::SetPosition(Vector2 newPosition)
{
	if (transform.position == newPosition)
	{
		return;
	}
	transform.position = Engine::GetLevel()->EvaluatePosition(this, newPosition);
}

void Object::SetAngle(float newAngle)
{
	transform.angle = clampAngle(newAngle);
}

