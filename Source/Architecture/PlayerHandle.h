#pragma once

#include "Object.h"

class Viewport;

class PlayerHandle : public Object
{
protected:
	Viewport* viewport = nullptr;
	Object* playerObject = nullptr;

protected:
	virtual void OnUpdate() override;

public:
	PlayerHandle() = default;
	virtual ~PlayerHandle() = default;

	void AssignObject(Object* playerObject);
	void AssignViewport(Viewport* viewport);

	Object* GetPlayerObject();
	Viewport* GetPlayerViewport();

};
