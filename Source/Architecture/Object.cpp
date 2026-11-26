#include "Object.h"

#include "Level.h"
#include "Structs/MathFuncs.h"
#include "Modules/Engine.h"


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

void Object::DetachFromHierarchy()
{
	vector<Object*> kids = children;
	for (Object* child : kids)
	{
		child->RemoveParent();
	}
	RemoveParent();
}

void Object::Start()
{
	lastFramePosition = transform.position;

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
	
	if (isActive)
	{
		frameDelta = Vector2(transform.position.x - lastFramePosition.x, transform.position.y - lastFramePosition.y);
		lastFramePosition = transform.position;

		float dt =Engine::GetDeltaTime();
		if (dt > 0.f)
		{
			velocity = Vector2(frameDelta.x / (dt * TILE_SIZE),
				frameDelta.y / (dt * TILE_SIZE));
		}

		size_t count = components.size();
		for (size_t i = 0; i < count; ++i)
		{
			if (components[i]->removed || components[i]->IsActive() == false)
			{
				continue;
			}
			components[i]->Update();
		}
		OnUpdate();
	}
	               
	SweepRemoved();
}

void Object::Destroy()
{
	level->Destroy(this);
}

bool Object::IsActive()
{
	return isActive;
}

void Object::SetActive(bool value)
{
	isActive = value;
}

bool Object::IsValid(Object* object) const
{
	return !GetLevel()->IsMarked(object);
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

Vector2 Object::GetVelocity() const
{
	return velocity;
}

float Object::GetSpeed() const
{
	return velocity.magnitude();
}

void Object::SetParent(Object* parent, Transform offset)
{
	if (parent == nullptr)
	{
		return;
	}
	if (this->parent != nullptr)
	{
		parent->RemoveChild(this);
	}
	this->parent = parent;
	this->parent->AddChild(this);
	localTransform = offset;
}

void Object::RemoveParent()
{
	if (parent != nullptr)
	{
		parent->RemoveChild(this);
	}
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

void Object::AddChild(Object* child)
{
	children.push_back(child);
}

void Object::RemoveChild(Object* child)
{
	children.erase(std::remove(children.begin(), children.end(), child), children.end());
}

std::vector<Object*> Object::GetChildren() const
{
	return children;
}

int Object::GetChildCount() const
{
	return (int)children.size();
}

Object* Object::GetChild(int index) const
{
	if (index < 0 || index >= (int)children.size())
	{
		return nullptr;
	}
	return children[index];
}

Level* Object::GetLevel() const
{
	return level;
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
	transform.position = level->EvaluatePosition(this, newPosition);
}

void Object::SetAngle(float newAngle)
{
	transform.angle = Math::clampAngle(newAngle);
}

