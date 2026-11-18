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
	PlayerHandle();

	void AssignObject(Object* playerObject);
	void AssignViewport(Viewport* viewport);

};
