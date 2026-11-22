#pragma once

#include "Architecture/PlayerHandle.h"

#include "Structs/Event.h"

class Player;

struct ControlScheme
{
	unsigned char forwardKey;
	unsigned char leftKey;
	unsigned char backKey;
	unsigned char rightKey;
	unsigned char interactKey;

	ControlScheme(unsigned char forwardKey, unsigned char leftKey, unsigned char backKey, unsigned char rightKey, unsigned char interactKey)
	{
		this->forwardKey = forwardKey;
		this->leftKey = leftKey;
		this->backKey = backKey;
		this->rightKey = rightKey;
		this->interactKey = interactKey;
	}

	static ControlScheme PlayerOne()
	{
		return ControlScheme('W', 'A', 'S', 'D', 'Q');
	}

	static ControlScheme PlayerTwo()
	{
		return ControlScheme(0x26, 0x25, 0x28, 0x27, 0x20);
	}
};

class BackdoomsPlayerHandle : public PlayerHandle
{
private:
	unsigned char forwardKey;
	unsigned char leftKey;
	unsigned char backKey;
	unsigned char rightKey;
	unsigned char interactKey;

	Vector2 movementInput;

	Player* playerRef = nullptr;

	Listener onPlayerDied;

	void OnPlayerDied();

protected:
	virtual void OnStart() override;
	virtual void OnUpdate() override;

public:
	BackdoomsPlayerHandle(Screenspace screenspace, ControlScheme controlScheme);
};
