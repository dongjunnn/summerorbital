#pragma once
#include "../../SceneInstance.h"
#include "../../Systems/Systems.h"
#include "../../Packet.h" 
#include "../../Events/EventManager.h"
#include <enet/enet.h>

class Server;
class EventManager;

// takes in a reference to server so it can use server info if need be
class ServerState
{
public:
	virtual ~ServerState() = default;

	virtual void onEnter(Server& server) = 0;
	virtual void handleEnetEvent(Server& server, ENetEvent& event) = 0;
	virtual void update(Server& server) = 0;
	virtual void broadcastStates(Server& server) = 0;
	virtual void onExit(Server& server) = 0;

protected:
	/* each server state has its own "sceneInstance", which is a container 
	   for entities, so each state is compartmentalised in that sense.

	   also initialises systems server states can use
	*/

	SceneInstance scene;

	MovementSystem movementSystem;
	CollisionSystem collisionSystem;
	EntityCleanSystem entityCleanSystem;

	ServerState()
		: movementSystem(scene),
		  collisionSystem(scene),
		  entityCleanSystem(scene)
	{ }

};