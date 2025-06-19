#pragma once
#include <SDL.h>
#include "../../Systems/Systems.h"
#include "../../SceneInstance.h"
#include "../../Packet.h"

class Client;

// base class for game states lol
class ClientState
{
public:
	virtual ~ClientState() = default;
	virtual void onEnter(Client& client) = 0;
	virtual void handleInput(Client& client) = 0;
	virtual void handleEnetEvent(Client& client, ENetEvent& event) = 0;
	virtual void update(Client& client) = 0;
	virtual void render(Client& client) = 0;
	virtual void onExit(Client& client) = 0;

protected:
	SceneInstance scene;

	ClickableSystem clickableSystem;
	RenderSystem renderSystem;
	MovementSystem movementSystem;
	CollisionSystem collisionSystem;
	EntityCleanSystem entityCleanSystem;

	ClientState()
		: clickableSystem(scene), 
		  renderSystem(scene), 
		  movementSystem(scene),
		  collisionSystem(scene),
		  entityCleanSystem(scene)
	{ }

};

