#pragma once

#include "Object.h"

#include "Modules/Window.h"

class Camera;
class Viewport;


class PlayerHandle : public Object
{
protected:
	Object* playerObject = nullptr;
	Camera* camera = nullptr;
	Viewport* viewport = nullptr;
	
	Screenspace screenspace;

	virtual void OnUpdate() override;

public:
	PlayerHandle(Screenspace screenspace);
	virtual ~PlayerHandle() = default;

	void AssignObject(Object* playerObject);
	void AssignCamera(Camera* camera);
	void AssignViewport(Viewport* viewport);

	Object* GetPlayerObject();
	Camera* GetCamera();
	Viewport* GetPlayerViewport();

};
